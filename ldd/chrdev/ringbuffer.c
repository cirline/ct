/**
 * RingBuffer
 * address: n-1 0 1 ... n-1
 * data:     *  * * ...  *
 *           |  |
 * position: rs ws          - read start & write start
 * when: w - r = 1, mean buffer empty
 * when: w - r = 0, mean buffer filled
 */

//#include <init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/ioctl.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/device.h>

#include "ringbuffer.h"

#define MEMSIZE     16
#define DEVNAME     "ringbuffer"

//#define RB_IOC_MAGIC  'k'
//#define RB_IOC_GET_MEMSIZE    _IOWR(RB_IOC_MAGIC, 0, int)

typedef struct ringbuffer {
    /* char device */
    struct cdev *cdev;
    struct class *cls;
    struct device *dev;
    /**
     * buffer & size
     * data pointer
     */
    char mem[MEMSIZE];
    int size;
    int read_pos;
    int write_pos;

    /* control */
    struct semaphore sem;
    wait_queue_head_t wq;
} rb_t;

static rb_t * rb;

/** initial:
 * buffer size,
 * read, write position
 */
static void rb_init(rb_t *rbptr)
{
    rbptr->size = MEMSIZE;
    rbptr->read_pos = rbptr->size - 1;
    rbptr->write_pos = 0;
    sema_init(&rbptr->sem, 1);
    init_waitqueue_head(&rbptr->wq);
}

/**
 * size of the buffer were used
 */
static inline size_t buffer_used(rb_t * rbptr)
{
    if(rbptr->write_pos > rbptr->read_pos)
        return rbptr->write_pos - rbptr->read_pos - 1;
    else
        return rbptr->write_pos - rbptr->read_pos - 1 + rbptr->size;
}

/**
 * size of the buffer not be used
 */
static inline size_t idle_buffer(rb_t *rbptr)
{
    return rbptr->size - buffer_used(rbptr) - 1;
}

static inline int next_read_pos(rb_t *rbptr)
{
    return (rbptr->read_pos + 1) % rbptr->size;
}

static inline int next_write_pos(rb_t *rbptr)
{
    return (rbptr->write_pos + 1) % rbptr->size;
}

static int is_empty(rb_t *rbptr)
{
    return (rbptr->read_pos + 1) % rbptr->size == rbptr->write_pos;
}

static inline int is_filled(rb_t *rbptr)
{
    return rbptr->write_pos == rbptr->read_pos;
}

static int rb_open(struct inode * node, struct file * filp)
{
    filp->private_data = rb;
    return 0;
}

static long rb_ioctl(struct file *filp, unsigned int cmd, unsigned long val)
{
    //struct ringbuffer *dev = filp->private_data;
    int ret = 0;
    int control_val;

    if(_IOC_TYPE(cmd) != RB_IOC_MAGIC) {
        printk("command error!\n");
        return -ENOTTY;
    }
    if(_IOC_DIR(cmd) & _IOC_READ)
        ret = ! access_ok(VERIFY_WRITE, (void __user *)val, _IOC_SIZE(cmd));
    else if(_IOC_DIR(cmd) & _IOC_WRITE)
        ret = ! access_ok(VERIFY_READ, (void __user *)val, _IOC_SIZE(cmd));
    if(ret) {
        printk("access verify failed!\n");
        return -EFAULT;
    }

    switch(cmd) {
        case RB_IOC_GET_MEMSIZE:
            if(!capable(CAP_SYS_ADMIN)) {
                printk("no permitted!\n");
                return -EPERM;
            }
            ret = __get_user(control_val, (int __user *)val);
            printk("control val = %d\n", control_val);
            control_val += MEMSIZE;
            ret = __put_user(control_val, (int __user *)val);
            break;
        default:
            printk("unknown command!\n");
            return -ENOTTY;
    }
    return ret;
}

//#define DEBUG_READ
#ifdef DEBUG_READ
#define debug_read(fmt, args...)   printk("ringbuffer: "fmt, ##args)
#else
#define debug_read(fmt, args...)
#endif
static ssize_t rb_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    struct ringbuffer *dev = filp->private_data;
    char * buffer;
    size_t buffer_pos;
    size_t buffer_size;
    size_t read_size;
    int ret;

    debug_read("read data size = %lu\n", size);
    if(size <= 0)
        return - EINVAL;
    buffer = kmalloc(size, GFP_KERNEL);
    if(!buffer)
        return - ENOMEM;

    buffer_pos = 0;
    while(buffer_pos < size) {
        if(down_interruptible(&dev->sem)) {
            ret = - ERESTARTSYS;
            goto out;
        }
        if(is_empty(dev)) {
            up(&dev->sem);
            break;
        }
        buffer_size = size - buffer_pos;
        read_size = (buffer_size < buffer_used(dev)) ? buffer_size : buffer_used(dev);
        while(read_size-- > 0) {
            dev->read_pos = next_read_pos(dev);
            buffer[buffer_pos++] = dev->mem[dev->read_pos];
        }
        up(&dev->sem);
        wake_up_interruptible(&dev->wq);
    }
    if(copy_to_user(buf, buffer, buffer_pos))
        ret = - ERESTARTSYS;
    else
        ret = buffer_pos;
out:
    kfree(buffer);
    return ret;
}

//#define DEBUG_WRITE
#ifdef DEBUG_WRITE
#define debug_write(fmt, args...)   printk("ringbuffer: "fmt, ##args)
#else
#define debug_write(fmt, args...)
#endif
static ssize_t rb_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    struct ringbuffer *dev = filp->private_data;
    char *buffer;
    int buffer_pos;
    int write_size;
    int buffer_size;
    int ret;

    debug_write("write data size: %lu\n", size);
    if(size <= 0)
        return -EINVAL;

    buffer = kmalloc(size, GFP_KERNEL);
    if(!buffer)
        return -ENOMEM;

    if(copy_from_user(buffer, buf, size)) {
        ret = - ERESTARTSYS;
        goto out;
    }
    debug_write("copy from use: %s\n", buffer);

    buffer_pos = 0;
    while(buffer_pos < size) {
        debug_write("buffer_pos = %d, size = %lu\n", buffer_pos, size);
        if(down_interruptible(&dev->sem)) {
            ret = -ERESTARTSYS; /* acquired semaphore failed */
            goto out;
        }
        if(is_filled(dev)) {
            up(&dev->sem);   /* buffer is filled, free sem and hang */
            //wake_up_interruptible(&dev->wq);
            if(wait_event_interruptible(dev->wq, !is_filled(dev))) {
                ret = - ERESTARTSYS;
                goto out;
            }
            if(down_interruptible(&dev->sem))
                continue;
        }
        buffer_size = size - buffer_pos; /* buffer to be writen size */
        write_size = (buffer_size < idle_buffer(dev)) ? buffer_size : idle_buffer(dev); /* size to be writen this loop */
        while(write_size-- > 0) {
            dev->mem[dev->write_pos] = buffer[buffer_pos++];    /* write data to current write_pos */
            debug_write("mem[%d] = %c\n", dev->write_pos, dev->mem[dev->write_pos]);
            dev->write_pos = next_write_pos(dev);               /* move the write_pos */
        }
        up(&dev->sem); /* a loop is finish */
        wake_up_interruptible(&dev->wq);
    }
    ret = size;
out:
    kfree(buffer);
    return ret;
}

struct file_operations rbfops = {
    .open   = rb_open,
    .unlocked_ioctl = rb_ioctl,
    .read   = rb_read,
    .write  = rb_write,
};


static int __init ringbuffer_init(void)
{
    int ret;
    dev_t devno;
    struct cdev *cdev;
    struct device *dev;
    struct class *cls;

    printk("%s\n", __func__);
    rb = kmalloc(sizeof(struct ringbuffer), GFP_KERNEL);
    if(!rb) {
        printk("kmalloc failure.\n");
        ret = - ENOMEM;
        goto out;
    }
    rb_init(rb);
    ret = alloc_chrdev_region(&devno, 0, 1, DEVNAME);
    if(ret < 0) {
        printk("alloc_chrdev_region failure.\n");
        goto out_free;
    }
    cdev = cdev_alloc();
    if(!cdev) {
        eprintk("cdev_alloc failure\n");
        ret = - ENOMEM;
        goto err_cdev_alloc;
    }
    //cdev_init(cdev, &rbfops);
    cdev->owner = THIS_MODULE;
    cdev->ops = &rbfops;
    kobject_set_name(&cdev->kobj, DEVNAME);
    ret = cdev_add(cdev, devno, 1);
    if(ret < 0) {
        eprintk("cdev_add failure.\n");
        goto err_cdev_add;
    }
    rb->cdev = cdev;

    cls = class_create(THIS_MODULE, DEVNAME"_cls");
    if(IS_ERR(cls)) {
        eprintk("class_create failure.\n");
        ret = PTR_ERR(cls);
        goto err_class_create;
    }
    rb->cls = cls;
    dev = device_create(cls, NULL, devno, NULL, DEVNAME);
    if(IS_ERR(dev)) {
        eprintk("device_create failure.\n");
        ret = PTR_ERR(dev);
        goto err_device_create;
    }
    return 0;
err_device_create:
    class_destroy(cls);
err_class_create:
    cdev_del(cdev);
err_cdev_add:
    kobject_put(&cdev->kobj);
err_cdev_alloc:
    unregister_chrdev_region(devno, 1);
out_free:
    kfree(rb);
out:
    return ret;
}

static void __exit ringbuffer_exit(void)
{
    device_destroy(rb->cls, rb->cdev->dev);
    class_destroy(rb->cls);
    cdev_del(rb->cdev);
    kobject_put(&rb->cdev->kobj);
    unregister_chrdev_region(rb->cdev->dev, 1);
    kfree(rb);
}

module_init(ringbuffer_init);
module_exit(ringbuffer_exit);
MODULE_LICENSE("GPL");



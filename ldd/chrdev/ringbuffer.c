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

#define MEMSIZE     128
#define DEVNAME     "ringbuffer"

#define RB_IOC_MAGIC  'k'
#define RB_IOC_GET_MEMSIZE    _IOWR(RB_IOC_MAGIC, 0, int)

typedef struct ringbuffer {
    struct cdev dev;
    char mem[MEMSIZE];
    int size;
    int read_pos;
    int write_pos;
    struct semaphore sem;
    wait_queue_head_t wq;
} rb_t;

static rb_t * rb;

static void rb_init(rb_t *rbptr)
{
    rbptr->size = MEMSIZE;
    rbptr->read_pos = rbptr->size - 1;
    rbptr->write_pos = 0;
    sema_init(&rbptr->sem, 1);
    init_waitqueue_head(&rbptr->wq);
}

static int idle_buffer(rb_t *rbptr)
{
    return rbptr->size - (rbptr->write_pos - rbptr->read_pos);
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

static ssize_t rb_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    struct ringbuffer *dev = filp->private_data;
    loff_t tmp = MEMSIZE - *ppos;
    printk("read- f_pos = %d, offset = %d\n", (int)filp->f_pos, (int)*ppos);
    if(tmp <= 0)
        return -1;
    if(size > tmp)
        size = tmp;
    if(copy_to_user(buf, dev->mem + *ppos, size))
        return -1;
    *ppos += size;
    printk("read+ f_pos = %d, offset = %d\n", (int)filp->f_pos, (int)*ppos);
    return size;
}

#define DEBUG_WRITE
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

    debug_write("write data size: %lu\n", size);
    if(size <= 0)
        return -EINVAL;

    buffer = kmalloc(size, GFP_KERNEL);
    if(!buffer)
        return -ENOMEM;

    if(copy_from_user(buffer, buf, size))
        return - ERESTARTSYS;
    debug_write("copy from use: %s\n", buffer);

    buffer_pos = 0;
    while(buffer_pos < size) {
        if(down_interruptible(&dev->sem))
            return -ERESTARTSYS; /* acquired semaphore failed */
        if(is_filled(dev)) {
            up(&dev->sem);   /* buffer is filled, free sem and hang */
            if(wait_event_interruptible(dev->wq, !is_filled(dev)))
                return -ERESTARTSYS;
            if(down_interruptible(&dev->sem))
                return -ERESTARTSYS;
        }
        buffer_size = size - buffer_pos; /* buffer to be writen size */
        write_size = (buffer_size < idle_buffer(dev)) ? buffer_size : idle_buffer(dev); /* size to be writen this loop */
        while(write_size-- > 0) {
            dev->mem[dev->write_pos] = buffer[buffer_pos++];    /* write data to current write_pos */
            debug_write("mem[%d] = %c\n", dev->write_pos, dev->mem[dev->write_pos]);
            dev->write_pos = next_write_pos(dev);               /* move the write_pos */
        }
        up(&dev->sem); /* a loop is finish */
    }
    return size;
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

    printk("%s\n", __func__);
    rb = kmalloc(sizeof(struct ringbuffer), GFP_KERNEL);
    if(!rb) {
        printk("kmalloc failure.\n");
        goto out;
    }
    rb_init(rb);
    ret = alloc_chrdev_region(&devno, 0, 1, DEVNAME);
    if(ret < 0) {
        printk("alloc_chrdev_region failure.\n");
        goto out_free;
    }
    cdev_init(&rb->dev, &rbfops);
    rb->dev.owner = THIS_MODULE;
    ret = cdev_add(&rb->dev, devno, 1);
    if(ret < 0) {
        printk("cdev_add failure.\n");
        goto out_free_devno;
    }
    return 0;
out_free_devno:
    unregister_chrdev_region(devno, 1);
out_free:
    kfree(rb);
out:
    return -1;
}

static void __exit ringbuffer_exit(void)
{
    cdev_del(&rb->dev);
    printk("devno = %d\n", rb->dev.dev);
    unregister_chrdev_region(rb->dev.dev, 1);
    kfree(rb);
}

module_init(ringbuffer_init);
module_exit(ringbuffer_exit);
MODULE_LICENSE("GPL");



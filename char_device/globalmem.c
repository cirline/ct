//#include <init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/poll.h>

#define MEMSIZE     16
#define DEVNAME     "globalmem"

struct globalmem {
    struct cdev dev;
    char mem[MEMSIZE];
} *gmp;

static int gm_open(struct inode * node, struct file * filp)
{
    filp->private_data = gmp;
    return 0;
}

static loff_t gm_llseek(struct file * filp, loff_t offset, int cur)
{
    loff_t tmp;
    switch(cur) {
        case SEEK_SET:
            if(offset < 0 || offset > MEMSIZE)
                return -1;
            filp->f_pos = offset;
            break;
        case SEEK_CUR:
            tmp = filp->f_pos + offset;
            if(tmp < 0 || tmp > MEMSIZE)
                return -1;
            filp->f_pos = tmp;
            break;
        case SEEK_END:
            if(offset > 0 || offset < -MEMSIZE)
                return -1;
            filp->f_pos = MEMSIZE + offset;
            break;
        default:
            return -1;
    }
    return filp->f_pos;
}

static long gm_ioctl(struct file *filp, unsigned int code, unsigned long val)
{
    return 0;
}

static ssize_t gm_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    struct globalmem *dev = filp->private_data;
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

static ssize_t gm_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    struct globalmem *dev = filp->private_data;
    loff_t tmp = MEMSIZE - *ppos;
    printk("write- f_pos = %d, offset = %d\n", (int)filp->f_pos, (int)*ppos);
    if(tmp <= 0)
        return -1;
    if(size > tmp)
        size = tmp;
    if(copy_from_user(dev->mem + *ppos, buf, size))
        return -1;
    *ppos += size;
    printk("write- f_pos = %d, offset = %d\n", (int)filp->f_pos, (int)*ppos);
    return size;
}

struct file_operations gmfops = {
    .open   = gm_open,
    .llseek = gm_llseek,
    .unlocked_ioctl = gm_ioctl,
    .read   = gm_read,
    .write  = gm_write,
};

static int __init globalmem_init(void)
{
    int ret;
    dev_t devno;

    printk("%s\n", __func__);
    gmp = kmalloc(sizeof(struct globalmem), GFP_KERNEL);
    if(!gmp) {
        printk("kmalloc failure.\n");
        goto out;
    }
    ret = alloc_chrdev_region(&devno, 0, 1, DEVNAME);
    if(ret < 0) {
        printk("alloc_chrdev_region failure.\n");
        goto out_free;
    }
    cdev_init(&gmp->dev, &gmfops);
    gmp->dev.owner = THIS_MODULE;
    ret = cdev_add(&gmp->dev, devno, 1);
    if(ret < 0) {
        printk("cdev_add failure.\n");
        goto out_free_devno;
    }
    return 0;
out_free_devno:
    unregister_chrdev_region(devno, 1);
out_free:
    kfree(gmp);
out:
    return -1;
}

static void __exit globalmem_exit(void)
{
    cdev_del(&gmp->dev);
    printk("devno = %d\n", gmp->dev.dev);
    unregister_chrdev_region(gmp->dev.dev, 1);
    kfree(gmp);
}

module_init(globalmem_init);
module_exit(globalmem_exit);
MODULE_LICENSE("GPL");



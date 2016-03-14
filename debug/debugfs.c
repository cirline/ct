#include <linux/module.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

static struct dentry * dbd;

int dbfs_open (struct inode * node, struct file * flip)
{
    printk("debugfs open\n");

    return 0;
}

int dbfs_close (struct inode * node , struct file *flip)
{
    printk("debugfs close\n");

    return 0;
}

ssize_t dbfs_read (struct file * flip, char __user * buf, size_t size, loff_t * offset)
{
    printk("debugfs read\n");
    return size;
}

ssize_t dbfs_write (struct file * flip, const char __user * buf, size_t size, loff_t * offset)
{
    char buffer[512];
    printk("debugfs write\n");

    if(copy_from_user(buffer, buf, size)) {}

    printk("buffer: %s\n", buffer);

    return size;
}

static const struct file_operations  debugfs_ops = {
    .open = dbfs_open,
    .read = dbfs_read,
    .write = dbfs_write,
    .release = dbfs_close,
};

static int __init debugfs_init(void)
{
    printk("debugfs test.\n");

    dbd = debugfs_create_file("debugfs_test", S_IFREG | S_IRUGO, NULL, NULL, &debugfs_ops);
    if(!dbd) {
        printk("debugfs_create_file failed.\n");
    }
    return 0;
}

static void __exit debugfs_exit(void)
{
    printk("debugfs exit.\n");

    debugfs_remove(dbd);
}

module_init(debugfs_init);
module_exit(debugfs_exit);
MODULE_LICENSE("GPL");


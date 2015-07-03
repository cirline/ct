#include <linux/module.h>


static int __init proc_file_init(void)
{
	printk(KERN_INFO"hello ---------add 0934------------------pc module\n");
	return 0;
}

static void __exit proc_file_exit(void)
{
	printk(KERN_INFO"bye kernel.\n");
}

module_init(proc_file_init);
module_exit(proc_file_exit);
MODULE_LICENSE("GPL");



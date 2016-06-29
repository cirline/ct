#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init mdl_init(void)
{
	printk(KERN_INFO"hello ---------add 0934------------------pc module\n");
	return 0;
}

static void __exit mdl_exit(void)
{
	printk(KERN_INFO"bye kernel.\n");
}

module_init(mdl_init);
module_exit(mdl_exit);



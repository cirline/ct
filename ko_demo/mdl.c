#define pr_fmt(fmt)	"" fmt

#include <linux/module.h>

static int __init _init(void)
{
	pr_info("init\n");
	return 0;
}

static void __exit _exit(void)
{
	pr_info("exit\n");
}

module_init(_init);
module_exit(_exit);
MODULE_LICENSE("GPL");



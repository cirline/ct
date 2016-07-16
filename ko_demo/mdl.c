#define pr_fmt(fmt)	"" fmt

#include <linux/module.h>

static int __init m_init(void)
{
	printk("init\n");
	return 0;
}

static void __exit m_exit(void)
{
	pr_info("exit\n");
}

module_init(m_init);
module_exit(m_exit);
MODULE_LICENSE("GPL");



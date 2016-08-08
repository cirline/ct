#include <linux/init.h>
#include <linux/module.h>

static __init int sm_init(void)
{
	printk("hello init\n");

    request_region();
    inb();

    request_mem_region();

    ioremap();
    iounmap();

    ioread8();
    iowrite8();

    ioport_map();
	return 0;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





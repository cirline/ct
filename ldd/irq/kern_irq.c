#include <linux/init.h>
#include <linux/module.h>

static __init int sm_init(void)
{
	printk("hello init\n");

    request_irq();
    free_irq();
    disable_irq();
    local_irq_save();
    local_irq_disable();
    local_irq_restore();
    local_irq_enable();
	return 0;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





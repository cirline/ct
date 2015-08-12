#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>

struct work_struct work;

void work_fn(struct work_struct *work)
{
    printk("------------- work fn ----------\n");
}

static __init int sm_init(void)
{

	printk("hello init\n");

    INIT_WORK(&work, work_fn);

    schedule_work(&work);

	return 0;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





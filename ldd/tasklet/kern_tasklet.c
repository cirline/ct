#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sched.h>

struct kern_tasklet {
    struct tasklet_struct tl;
    char data[32];
    wait_queue_head_t wq;
};

struct kern_tasklet kt;

void task_a(unsigned long data)
{
    printk("-----------%s----------\n", __func__);
}

static __init int sm_init(void)
{
	printk("hello init\n");

    tasklet_init(&kt.tl, task_a, (unsigned long)&kt);
    printk("schedule task a: \n");
    tasklet_schedule(&kt.tl);
    schedule_timeout(500);
    printk("disable task a:\n");
    tasklet_disable(&kt.tl);
    printk("schedule task a: \n");
    tasklet_schedule(&kt.tl);
    schedule_timeout(500);
    printk("enable task a:\n");
    tasklet_enable(&kt.tl);

	return 0;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");






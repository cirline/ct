#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/delay.h>

static struct work_struct module_work1, module_work2;
static struct workqueue_struct * module_works_queue;
static struct workqueue_struct * module_works_queue2;
static spinlock_t lock1, lock2;

static void module_work1_func(struct work_struct *work)
{

	int i;
    printk("hello ws1. \n");
	for(i = 0; i < 8; i++) {
        printk("work1 try lock1 .\n");
		spin_lock(&lock1);
        printk("work1 get lock1 .\n");
		printk("work1 module loop %d, release lock1\n", i);
        spin_unlock(&lock1);
	}
}

static void module_work2_func(struct work_struct *work)
{
	int i;
    printk("hello ws2. \n");
	for(i = 0; i < 8; i++) {
        printk("work2 try lock2 .\n");
        spin_lock(&lock2);
        printk("work2 get lock2 .\n");
		printk("work2 module loop %d, release lock2\n", i);
        spin_unlock(&lock2);
	}
}

static int __init module_init_func(void)
{
    printk("hello module\n");

    spin_lock_init(&lock1);
    spin_lock_init(&lock2);
    printk("init lock end.\n");
    spin_lock(&lock2);
    printk("locked lock2 .\n");

    module_works_queue = create_singlethread_workqueue("module_works_queue");
    if(!module_works_queue) {
        printk("create_singlethread_workqueue failed! \n");
        return -1;
    }

    module_works_queue2 = create_singlethread_workqueue("module_works_queue2");
    if(!module_works_queue2) {
        printk("create_singlethread_workqueue2 failed! \n");
		goto create_thread_queue2_failed;
    }

    INIT_WORK(&module_work1, module_work1_func);
    INIT_WORK(&module_work2, module_work2_func);

	printk("start work1.\n");
    queue_work(module_works_queue, &module_work1);
	printk("start work2.\n");
    queue_work(module_works_queue2, &module_work2);

    return 0;
create_thread_queue2_failed:
	destroy_workqueue(module_works_queue);
	return -1;
}

static void __exit module_exit_func(void)
{
    printk("bye, module\n");
    destroy_workqueue(module_works_queue);
    destroy_workqueue(module_works_queue2);
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

static struct work_struct module_work1, module_work2;
static struct workqueue_struct * module_works_queue;
static struct workqueue_struct * module_works_queue2;
static struct semaphore sem1, sem2;

static void module_work1_func(struct work_struct *work)
{
	int i;
    printk("hello ws1. \n");
	for(i = 0; i < 8; i++) {
		if(down_interruptible(&sem1)) {
			printk("down sem failed.\n");
			return;
		}
		printk("work1 module loop %d\n", i);
		up(&sem2);
	}
}

static void module_work2_func(struct work_struct *work)
{
	int i;
    printk("hello ws2. \n");
	for(i = 0; i < 8; i++) {
		if(down_interruptible(&sem2)) {
			printk("down sem failed.\n");
			return;
		}
		printk("work2 module loop %d\n", i);
		up(&sem1);
	}
}

static int __init module_init_func(void)
{
    printk("hello module\n");

	sema_init(&sem1, 1);
	sema_init(&sem2, 0);

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
    destroy_workqueue(module_works_queue1);
    destroy_workqueue(module_works_queue2);
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


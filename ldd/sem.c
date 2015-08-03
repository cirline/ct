#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/semaphore.h>

static struct workqueue_struct * module_works_queue;
static struct work_struct module_work1, module_work2;
static struct semaphore sem1, sem2;

static void module_work1_func(struct work_struct *work)
{
    int i;

    for(i = 0; i < 8; i++) {
        if(down_interruptible(&sem1)) {
            printk("work1 require sem1 failed.\n");
            continue;
        }
        printk("work1, %d\n", i);
        up(&sem2);
    }
}

static void module_work2_func(struct work_struct *work)
{
    int i;

    for(i = 0; i < 8; i++) {
        if(down_interruptible(&sem2)) {
            printk("work2 require sem2 failed.\n");
            continue;
        }
        printk("work2, %d\n", i);
        up(&sem1);
    }
}

static int __init module_init_func(void)
{
    printk("hello module\n");

    sema_init(&sem1, 1);
    sema_init(&sem2, 1);
    /*
    if(down_interruptible(&sem2)) {
        printk("down the sem2 failed.\n");
        goto out;
    }
    */

    module_works_queue = create_singlethread_workqueue("module_works_queue");
    if(!module_works_queue) {
        printk("create_singlethread_workqueue failed! \n");
        return -1;
    }

    INIT_WORK(&module_work1, module_work1_func);
    INIT_WORK(&module_work2, module_work2_func);

    queue_work(module_works_queue, &module_work1);
    queue_work(module_works_queue, &module_work2);

    return 0;
out:
    return -1;
}

static void __exit module_exit_func(void)
{
    printk("bye, module\n");
    destroy_workqueue(module_works_queue);
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


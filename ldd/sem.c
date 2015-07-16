#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>

static struct work_struct module_work1;
static struct workqueue_struct * module_works_queue;

static void module_work1_func(struct work_struct *work)
{
    printk("hello ws1. \n");
}

static int __init module_init_func(void)
{
    printk("hello module\n");

    module_works_queue = create_singlethread_workqueue("module_works_queue");
    if(!module_works_queue) {
        printk("create_singlethread_workqueue failed! \n");
        return -1;
    }

    INIT_WORK(&module_work1, module_work1_func);

    queue_work(module_works_queue, &module_work1);

    return 0;
}

static void __exit module_exit_func(void)
{
    printk("bye, module\n");
    destroy_workqueue(module_works_queue);
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


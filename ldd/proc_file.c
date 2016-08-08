#include <linux/init.h>
#include <linux/module.h>

static struct work_struct proc_ws;
static struct workqueue_struct proc_wq;

static void proc_ws1(struct work_struct)
{
    printk("hello ws1. \n");
}

static int __init proc_init(void)
{
    printk("hello module\n");

//   proc_wq = create_singlethread_workqueue()

//    INIT_WORK(&pr);

    return 0;
}

static void __exit proc_exit(void)
{
    printk("bye, module\n");
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");


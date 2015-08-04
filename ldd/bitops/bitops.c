/**
 * NOTE !!!
 * spin_lock & spin_unlock must be used by pair
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <asm/atomic.h>
#include <linux/slab.h>
#include <asm/bitops.h>

#define DEBUG_BITOPS
#ifdef DEBUG_BITOPS
#define print_bitops(format, args...)   printk("[bitops] "format, ##args)
#else
#define print_bitops(format, args...)
#endif

static struct work_struct module_work1, module_work2;
static struct workqueue_struct * module_works_queue;
static struct workqueue_struct * module_works_queue2;
static spinlock_t lock1, lock2;

typedef struct module_struct {
    atomic_t        atomic;
    unsigned long   soft_register;
} mod_t;

static mod_t * mptr;

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

    mptr = (mod_t *)kmalloc(sizeof(mod_t), GFP_KERNEL);
    if(!mptr) {
        printk(KERN_ERR"malloc struct failed !!!\n");
        goto malloc_failed;
    }

#ifdef DEBUG_ATOMIC
    atomic_set(&mptr->atomic, 8);
    printk("set atomic : %d\n", atomic_read(&mptr->atomic));
    atomic_add(2, &mptr->atomic);
    printk("add atomic 2 : %d\n", atomic_read(&mptr->atomic));
    atomic_sub(6, &mptr->atomic);
    printk("sub atomic 6 : %d\n", atomic_read(&mptr->atomic));
    atomic_inc(&mptr->atomic);
    printk("inc atomic : %d\n", atomic_read(&mptr->atomic));
    atomic_dec(&mptr->atomic);
    printk("dec atomic : %d\n", atomic_read(&mptr->atomic));

    while(!atomic_dec_and_test(&mptr->atomic)) {
        printk("dec & test atomic : %d\n", atomic_read(&mptr->atomic));
    }
#endif

#ifdef DEBUG_BITOPS
    mptr->soft_register = 0;
    set_bit(1, &mptr->soft_register);
    print_bitops("set bit[1] : 0x%lx\n", mptr->soft_register);
    clear_bit(1, &mptr->soft_register);
    print_bitops("clear bit[1] : 0x%lx\n", mptr->soft_register);
    change_bit(5, &mptr->soft_register);
    print_bitops("change bit[5] : 0x%lx\n", mptr->soft_register);
    if(test_bit(5, &mptr->soft_register))
        print_bitops("test bit[5] : yes(0x%lx)\n", mptr->soft_register);
    else
        print_bitops("test bit[5] : no(0x%lx)\n", mptr->soft_register);
#endif

    spin_lock_init(&lock1);
    spin_lock_init(&lock2);
    printk("init lock end.\n");

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

    printk("__init try lock.\n");
    spin_lock(&lock1);
    printk("__init locked!\n");
    spin_unlock(&lock1);
    printk("__init release lock.\n");

	printk("start work1.\n");
    //queue_work(module_works_queue, &module_work1);
	printk("start work2.\n");
    //queue_work(module_works_queue2, &module_work2);

    return 0;
create_thread_queue2_failed:
	destroy_workqueue(module_works_queue);
    kfree(mptr);
malloc_failed:
	return -1;
}

static void __exit module_exit_func(void)
{
    printk("bye, module\n");
    destroy_workqueue(module_works_queue);
    destroy_workqueue(module_works_queue2);
    kfree(mptr);
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


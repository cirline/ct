#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

static int __init module_init_func(void)
{
    unsigned long prev_time, now_time, next_time;
    int n;

    for(n = 0; n < 10; n++) {
        now_time = jiffies;
        prev_time = now_time;
        next_time = now_time + HZ;
        for(; now_time < next_time; now_time = jiffies)
            ;
        printk("[%d] ------ [%lu]\n", n, now_time);
    }
    return 0;

}

static void __exit module_exit_func(void)
{
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


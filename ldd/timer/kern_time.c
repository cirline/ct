#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/wait.h>

struct kern_timer {
    unsigned long prev_jiffies;
    wait_queue_head_t wq;
    int loop;
    char buf[128];
    struct timer_list timer;
};

struct kern_timer t;

void timer_fn(unsigned long data)
{
    //unsigned long j = jiffies;
    struct kern_timer *p = (struct kern_timer *)data;

    p->loop = 1;
    strcpy(p->buf, "hello timer!");
    //printk("timer_fn, now_jiffies = %lu, prev_jiffies = %lu\n", j, p->prev_jiffies);
    wake_up_interruptible(&p->wq);
}

static int __init module_init_func(void)
{
    unsigned long next_time;
    struct timespec time;
    //

    init_waitqueue_head(&t.wq);
    t.loop = 0;

    time.tv_sec = 5;
    time.tv_nsec = 0;

    next_time = timespec_to_jiffies(&time);
    printk("next_time = %lu\n", next_time);

    init_timer(&t.timer);
    t.prev_jiffies = jiffies;
    t.timer.expires = t.prev_jiffies + next_time;
    t.timer.data = (unsigned long) &t;
    t.timer.function = timer_fn;

    printk("expires time = %lu, set time = %lu\n", t.timer.expires, t.prev_jiffies);

    add_timer(&t.timer);

    wait_event_interruptible(t.wq, t.loop);

    printk("buf: %s\n", t.buf);

#if 0
    for(n = 0; n < 10; n++) {
        now_time = jiffies;
        prev_time = now_time;
        next_time = now_time + HZ;
        for(; now_time < next_time; now_time = jiffies)
            ;
        jiffies_to_timespec(now_time, &time);
        printk("sec = %d, nano sec = %d\n", time.tv_sec, time.tv_nsec);
    }
#endif
    return 0;

}

static void __exit module_exit_func(void)
{
}

module_init(module_init_func);
module_exit(module_exit_func);
MODULE_LICENSE("GPL");


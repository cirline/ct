#define pr_fmt(fmt)	"jiffies " fmt

#include <linux/module.h>
#include <linux/delay.h>

static int __init jiffies_init(void)
{
	unsigned long now, time1, time2;
	
	now = jiffies;
	pr_info("init\n");
	time1 = jiffies;
	msleep(10);
	time2 = jiffies;

	pr_info("HZ = %d, now = %lu, time1 = %lu, time2 = %lu\n", HZ, now, time1 - now, time2 - time1);

	return 0;
}

static void __exit jiffies_exit(void)
{
	pr_info("exit\n");
}

module_init(jiffies_init);
module_exit(jiffies_exit);
MODULE_LICENSE("GPL");



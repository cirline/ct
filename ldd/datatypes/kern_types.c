#include <linux/init.h>
#include <linux/module.h>

static __init int sm_init(void)
{
	printk("hello init\n");
	return 0;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
    printk("u8, u16, u32, u64 size: %d, %d, %d, %d\n", sizeof(u8), sizeof(u16), sizeof(u32), sizeof(u64));

    ERR_PTR();

    struct list_head list;
    INIT_LIST_HEAD();
    list_add();
    list_add_tail();
    list_del();
    list_empty();
    list_entry();
    list_for_each();
    list_for_each_entry();
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





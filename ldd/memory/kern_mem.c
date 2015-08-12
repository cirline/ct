#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

struct kmem_cache *kcache;
char * buf;

static __init int sm_init(void)
{
	printk("hello init\n");

    kcache = kmem_cache_create("kmem_cache", 512, 0, SLAB_HWCACHE_ALIGN, NULL);
    if(!kcache) {
        printk("kmem_cache_create failed !!! \n");
        return - ENOMEM;
    }

    buf = kmem_cache_alloc(kcache, GFP_KERNEL);
    if(!buf) {
        printk("kmem_cache_alloc failed !!!\n");
        return - ENOMEM;
    }

    memcpy(buf, "hello kmem cache !", 32);

    printk("buf: %s\n", buf);


	return 0;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
    kmem_cache_free(kcache, buf);
    kmem_cache_destroy(kcache);
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





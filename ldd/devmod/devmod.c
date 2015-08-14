#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/stat.h>

int kernsm_match(struct device *dev, struct device_driver *drv)
{
    printk("run kernsm bus match ...\n");
    return 0;
}

ssize_t kernsm_bus_show(struct bus_type *bus, char *buf)
{
    printk("run kernsm bus attr show ...\n");
    return snprintf(buf, PAGE_SIZE, "%s\n", "sm v1.0");
}

ssize_t kernsm_bus_store(struct bus_type *bus, const char *buf, size_t count)
{
    printk("run kernsm bus attr store, size %lu ...\n", count);
    printk("%s\n", buf);
    return count;
}

struct bus_attribute kernsm_bus_attr = {
    .attr = { .name = "kernsm_bus_attr_name", .mode = S_IRUGO | S_IWUSR, },
    .show = kernsm_bus_show,
    .store = kernsm_bus_store,
};

//BUS_ATTR();

struct bus_type kernsm_bus_type = {
    .name = "kernsm",
    .match = kernsm_match,
    //.hotplug = kernsm_hotplug,
};

static __init int sm_init(void)
{
    int ret;

	printk("hello init\n");
    ret = bus_register(&kernsm_bus_type);
    if(ret) {
        printk("failed to register bus type kernsm\n");
        return -1;
    }

    ret = bus_create_file(&kernsm_bus_type, &kernsm_bus_attr);
    if(ret) {
        printk("bus create file attr failed !!!\n");
        goto create_attr_failed;
    }


	return 0;

create_attr_failed:
    bus_unregister(&kernsm_bus_type);
    return -1;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
    bus_remove_file(&kernsm_bus_type, &kernsm_bus_attr);
    bus_unregister(&kernsm_bus_type);
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/stat.h>

int kernsm_match(struct device *dev, struct device_driver *drv)
{
    printk("run kernsm bus match ...\n");
    printk("device name %s, driver name %s\n", dev->init_name, drv->name);
    return 1;
//    return ! strcmp(dev->init_name, drv->name);
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

void kernsm_bus_release(struct device *dev)
{
    printk("kernsm bus release. \n");
}

struct device kernsm_bus = {
    .init_name = "kernsm_bus0",
    .release = kernsm_bus_release,
};

int kernsm_driver_probe(struct device *dev)
{
    printk("kernsm name : %s, driver probe ... \n", dev->init_name);
    return 0;
}

struct _kernsm_driver {
    char *version;
    struct module *module;
    struct device_driver driver;
    struct driver_attribute drv_attr;
};

struct _kernsm_driver kernsm_driver = {
    .version = "kernsm driver v1",
    .module = THIS_MODULE,
    .driver = {
        .name = "kernsm_driver",
        .probe = kernsm_driver_probe,
    },
};

ssize_t kernsm_driver_attr_show(struct device_driver *driver, char *buf)
{
    struct _kernsm_driver *drv = container_of(driver, struct _kernsm_driver, driver);
    return snprintf(buf, PAGE_SIZE, "%s\n", drv->version);
}

/* device */
struct _kernsm_device {
    char *version;
    char *name;
    struct _kernsm_driver *driver;
    struct device device;
    struct device_attribute dev_attr;
};

void kernsm_device_release(struct device *dev)
{
    printk("kernsm release device ..\n");
}

struct _kernsm_device kernsm_device = {
    .version = "kernsm device v1",
    .name = "kernsm_driver",
    .device = {
        .init_name = "kernsm_device_init_name",
        .bus = &kernsm_bus_type,
        .parent = &kernsm_bus,
        .release = kernsm_device_release,
    },
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

    /* bus device */
    ret = device_register(&kernsm_bus);
    if(ret) {
        printk("register bus device failed !!!\n");
        goto register_bus_device_failed;
    }

    /* driver */
    kernsm_driver.driver.bus = &kernsm_bus_type;
    ret = driver_register(&kernsm_driver.driver);
    if(ret) {
        printk("kernsm driver register failed !!! \n");
        goto register_driver_failed;
    }
    kernsm_driver.drv_attr.attr.name = "version";
//    kernsm_driver.drv_attr.attr.owner = kernsm_driver.module;
    kernsm_driver.drv_attr.attr.mode = S_IRUGO | S_IWUSR;
    kernsm_driver.drv_attr.show = kernsm_driver_attr_show;
    kernsm_driver.drv_attr.store = NULL;
    ret = driver_create_file(&kernsm_driver.driver, &kernsm_driver.drv_attr);
    if(ret) {
        printk("kernsm driver create file failed !!! \n");
        goto driver_create_file_failed;
    }

    /* device */
//    kernsm_device.device.init_name = kernsm_device.name;
    printk("__init device name2: %s\n", kernsm_device.device.init_name);
    ret = device_register(&kernsm_device.device);
    if(ret) {
        printk("register kernsm device failed !!! \n");
        goto register_device_failed;
    }

	return ret;

register_device_failed:
    driver_remove_file(&kernsm_driver.driver, &kernsm_driver.drv_attr);
driver_create_file_failed:
    driver_unregister(&kernsm_driver.driver);
register_driver_failed:
    device_unregister(&kernsm_bus);
register_bus_device_failed:
    bus_remove_file(&kernsm_bus_type, &kernsm_bus_attr);
create_attr_failed:
    bus_unregister(&kernsm_bus_type);
    return ret;
}

static __exit void sm_exit(void)
{
	printk("bye, exit\n");
    device_unregister(&kernsm_device.device);
    driver_remove_file(&kernsm_driver.driver, &kernsm_driver.drv_attr);
    driver_unregister(&kernsm_driver.driver);
    device_unregister(&kernsm_bus);
    bus_remove_file(&kernsm_bus_type, &kernsm_bus_attr);
    bus_unregister(&kernsm_bus_type);
}

module_init(sm_init);
module_exit(sm_exit);
MODULE_LICENSE("GPL");





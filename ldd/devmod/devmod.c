#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/stat.h>
//#include <linux/cdev.h>
#include <linux/fs.h>

#define debug(format, args...) printk("%d: "format, __LINE__, ##args)

int kernsm_match(struct device *dev, struct device_driver *drv)
{
    printk("bus match .. device name %s, driver name %s\n", dev->kobj.name, drv->name);
    if(dev->kobj.name && drv->name)
        return ! strcmp(dev->kobj.name, drv->name);
    else
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

void kernsm_bus_release(struct device *dev)
{
    printk("kernsm bus release. \n");
}

struct device kernsm_bus = {
    .init_name = "kernsm_bus0",
    .release = kernsm_bus_release,
};


/* class */
#define DEVNAME     "kernsm_device_name"
void class_device_release(struct device *dev) {
    debug("class_device_release in .\n");
}

void kernsm_class_release(struct class *class) {
    debug("kernsm_class_release in .\n");
}

struct class kernsm_class = {
    .name = "kernsm_class",
    .dev_release = class_device_release,
    .class_release = kernsm_class_release,
};

dev_t devno;
struct device *kdev;
int kernsm_driver_probe(struct device *dev)
{
    int ret;

    printk("kernsm name : %s, driver probe ... \n", dev->kobj.name);

    /* device id */
    ret = alloc_chrdev_region(&devno, 0, 1, DEVNAME);
    if(ret < 0) {
        printk("alloc devno failed !!!\n");
        goto alloc_devno_failed;
    }
    debug("devno: %u\n", devno);

    /* class */
    ret = class_register(&kernsm_class);
    if(ret) {
        printk("register class failed !!! \n");
        goto register_class_failed;
    }

    kdev = device_create(&kernsm_class, NULL, devno, NULL, "kernsm_dev0");
    if(!kdev) {
        printk("device create failed !!!\n");
        goto device_create_failed;
    }

    return 0;

device_create_failed:
    class_unregister(&kernsm_class);
register_class_failed:
    unregister_chrdev_region(devno, 1);
alloc_devno_failed:
    return ret;
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
    kernsm_device.device.init_name = kernsm_device.name;
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
    unregister_chrdev_region(devno, 1);
    class_unregister(&kernsm_class);
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





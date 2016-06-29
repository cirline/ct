//#define DEBUG

#define pr_fmt(fmt)  "iogrp: " fmt

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

#include "iogrp.h"

#define IOGRP_NAME  "iogrp"

#define KERN_CAR_STATE_STOP     0
#define KERN_CAR_STATE_HIRED    1
#define KERN_CAR_STATE_FORHIRE  2
#define KERN_CAR_STATE_TASK     3

#define IOGRP_SUSPEND_STATE_LOW		0
#define IOGRP_SUSPEND_STATE_HIGH	1
#define IOGRP_SUSPEND_STATE_KEEP	2

struct iogrp_desc {
    const char *label;
    int gpio;
    int keycode;
    int value;
    int suspend_state;
    struct device_node *node;
};

struct iogrp_dev {
    struct cdev *cdev;
    struct class *cls;
    struct device *dev;
    dev_t devno;
    struct device_node *node;

    struct pinctrl *pinctrl;

    struct iogrp_desc *iolist;
    int lsize;

    int carstate;
    int timer_flag;
    struct timer_list s_timer;
};

static struct iogrp_dev *iodev;

extern int (*leds_ctrl)(int keycode, int on);

static ssize_t keys_leds_show_addr(struct device *dev, struct device_attribute *attr, char *buf)
{
	int i;
	int len;
	char *p = buf;

	for(i = 0, len = 0; i < iodev->lsize; i++) {
		struct iogrp_desc *io = iodev->iolist + i;
		if(gpio_is_valid(io->gpio))
			len += sprintf(p, "%12s = %d\n", io->label, gpio_get_value(io->gpio));
		else
			len += sprintf(p, "invalid gpio %s\n", io->label);
		p = buf + len;
	}
	return len;
}

static struct iogrp_desc * iogrp_find_name(char *name)
{
	int i;

	for(i = 0; i < iodev->lsize; i++) {
		struct iogrp_desc *io = iodev->iolist + i;
		if(strcmp(io->label, name) == 0)
			return io;
	}

	return NULL;
}

static int iogrp_force_output(struct iogrp_desc *iod, int gval)
{
    int rc;
    int gpio = iod->gpio;

    if(!gpio_is_valid(gpio)) {
        pr_err("unvalid gpio (%d)\n", gpio);
        return -1;
    }

    gval = !!gval; 
    gpio_set_value(gpio, gval);

    rc = gpio_get_value(gpio);
    if(rc == gval) {
        pr_debug("gpio (%d) set %d success\n", gpio, gval);
        return 0;
    }

    gpio_free(gpio);
    rc = gpio_request(gpio, iod->label);
    if(rc) {
        pr_err("re request gpio failed.\n");
        return -1;
    }
    return gpio_direction_output(gpio, gval);
}


static ssize_t keys_leds_store_addr(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int ret;
    char name[16];
    int value;
    struct iogrp_desc *p;

    ret = sscanf(buf, "%s %d", name, &value);
    pr_debug("store: name = %s, value = %d, ret = %d\n", name, value, ret);
    if(ret < 2) {
        pr_err("usage: echo ioname [value]\n");
    } else {
        p = iogrp_find_name(name);
        if(p)
            iogrp_force_output(p, !!value);
    }

    /* do while */
    return count;
}

static DEVICE_ATTR(addr, S_IRUGO | S_IWUGO, keys_leds_show_addr, keys_leds_store_addr);

static struct attribute *keys_leds_attrs[] = {
    &dev_attr_addr.attr,
    NULL,
};

static struct attribute_group keys_leds_attr_group = {
    .attrs = keys_leds_attrs,
};

static struct iogrp_desc * keys_leds_find(int keycode)
{
	int i;

	for(i = 0; i < iodev->lsize; i++) {
		struct iogrp_desc *io = iodev->iolist + i;
		if(io->keycode == keycode)
			return io;
	}

	return NULL;
}

static int iogrp_get_carstate_led(void)
{
    struct iogrp_desc *io1, *io2;
    int ssw1 = 0;
    int ssw2 = 0;
    int ssw;

    //pr_debug("get car state led.\n");
    io1 = iogrp_find_name("ssw1");
    io2 = iogrp_find_name("ssw2");

    if(io1)
        ssw1 = gpio_get_value(io1->gpio);
    if(io2)
        ssw2 = gpio_get_value(io2->gpio);
    ssw = ((ssw2 & 1) << 1) | (ssw1 & 1);

    //pr_debug("ssw1 = %d, ssw2 = %d\n", ssw1, ssw2);

    return ssw;
}

static int iogrp_set_carstate_led(int state)
{
    struct iogrp_desc *iossw1, *iossw2, *iolat0, *iolat1, *iometersw;
    /* bit 0 -> n: ssw1, ssw2, lat0, lat1, msw */
    int iostate;

    pr_debug("set car state = %d\n", state);

    iossw1 = iogrp_find_name("ssw1");
    iossw2 = iogrp_find_name("ssw2");
    iolat0 = iogrp_find_name("latio0");
    iolat1 = iogrp_find_name("latio1");
    iometersw = iogrp_find_name("metersw");
    switch(state) {
        case KERN_CAR_STATE_STOP: /* stop */
            iostate = 0 | (0<<1) | (0<<2) | (0<<3) | (0<<4);
            break;
        case KERN_CAR_STATE_HIRED: /* hired */
            iostate = 1 | (0<<1) | (1<<2) | (0<<3) | (1<<4);
            break;
        case KERN_CAR_STATE_FORHIRE: /* for hire */
            iostate = 0 | (1<<1) | (0<<2) | (1<<3) | (0<<4);
            break;
        case KERN_CAR_STATE_TASK: /* task */
            iostate = 1 | (1<<1) | (1<<2) | (1<<3) | (0<<4);
            break;
        default:
            pr_err("set car state failed, state = %d\n", state);
            return -1;
    }
    iodev->carstate = state;
    pr_debug("iostate = 0x%x\n", iostate);
    if(iossw1)
        iogrp_force_output(iossw1, !!(iostate & 0x1));
    if(iossw2)
        iogrp_force_output(iossw2, !!(iostate & 0x2));
    if(iolat0)
        iogrp_force_output(iolat0, !!(iostate & 0x4));
    if(iolat1)
        iogrp_force_output(iolat1, !!(iostate & 0x8));
    if(iometersw)
        iogrp_force_output(iometersw, !!(iostate & 0x10));

    return 0;
}

int gpio_keys_leds_ctrl(int keycode, int on)
{
	struct iogrp_desc *led;

    pr_debug("keycode = %d, state = %s\n", keycode, on ? "on" : "off");

    if(keycode == 88)
	    return 0;

    led = keys_leds_find(keycode);
    if(!led) {
	    pr_debug("cannot found keycode = %d\n", keycode);
	    return - EINVAL;
    }

    if(!gpio_is_valid(led->gpio)) {
        pr_err("gpio is not valid, %d, %s\n", led->gpio, led->label);
        return - EINVAL;
    }

    gpio_set_value_cansleep(led->gpio, !on);

    return 0;
}

static int iogr_pinctrl_config(int active)
{
	struct pinctrl_state *state;
	int rc;

	pr_debug("set pinctrl config = %d\n", active);
	if(active) {
		state = pinctrl_lookup_state(iodev->pinctrl, "tlmm_iogr_active");
		if(IS_ERR(state)) {
			pr_err("get pinctrl active failed.\n");
			return PTR_ERR(state);
		}
	} else {
		state = pinctrl_lookup_state(iodev->pinctrl, "tlmm_iogr_suspend");
		if(IS_ERR(state)) {
			pr_err("get pinctrl suspend failed.\n");
			return PTR_ERR(state);
		}
	}

	rc = pinctrl_select_state(iodev->pinctrl, state);
	if(rc) {
		pr_err("set pinctrl state failed.\n");
		return rc;
	}

	return 0;
}

static int iogrp_prase_dt(struct device_node *node)
{
	struct device_node *pp;
	struct iogrp_desc *io;
	int i;
	int error;
	int size;

	size = of_get_child_count(node);
	pr_debug("io num = %d\n", size);
	if(size <= 0) {
		error = - ENODEV;
		goto err_out;
	}
	iodev->lsize = size;

	io = kzalloc(sizeof(*io) * size, GFP_KERNEL);
	if(!io) {
		pr_err("alloc iolist failed.\n");
		goto err_out;
	}
	iodev->iolist = io;

	i = 0;
	for_each_child_of_node(node, pp) {
		int gpio;

		io = iodev->iolist + i;

		io->label = of_get_property(pp, "label", NULL);

		gpio = of_get_named_gpio(pp, "gpios", 0);
		if(gpio < 0) {
			pr_err("get gpio failed, i = %d\n", i);
			error = gpio;
			goto err_out;
		}
		io->gpio = gpio;

		if(of_property_read_u32(pp, "keycode", &io->keycode))
			io->keycode = 0;

		io->value = of_find_property(pp, "boot,set", NULL) ? 1 : 0;

		if(of_property_read_u32(pp, "suspend,state", &io->suspend_state))
			io->suspend_state = 0;

		pr_debug("[%2d]gpio %s : keycode %d, default %d, suspend state %d\n",
				i, io->label, io->keycode, io->value, io->suspend_state);

		i++;
	}
	pr_debug("node read: (%d/%d)\n", i, size);

	return i;

err_out:
	return error;
}

static int iogrp_open(struct inode * node, struct file * filp)
{
    pr_debug("iogrp_open ...\n");
    return 0;
}

static long iogrp_ioctl(struct file *filp, unsigned int cmd, unsigned long vp)
{
    int ret;
    int ioval;
    struct iogrp_desc *p, *pp;
    int pio, ppio;
    unsigned long val = vp;

    pr_debug("iogrp_ioctl, cmd = %d ...\n", cmd);
    switch(cmd) {
        case CMD_SET_CAR_STATE:
            ret = __get_user(ioval, (int __user *)val);
            pr_debug("read ioval = %d\n", ioval);
            iogrp_set_carstate_led(ioval);
            break;
        case CMD_SET_METER_HIRED:
            ret = __get_user(ioval, (int __user *)val);
            pr_debug("read ioval = %d\n", ioval);
            p = iogrp_find_name("metersw");
            if(p)
                iogrp_force_output(p, !!ioval);
            break;
        case CMD_GET_CAR_STATE:
            {
                int ssw;
                ssw = iogrp_get_carstate_led();
                pr_debug("get car state %d\n", ssw);
                __put_user(ssw, (int __user *)val);
            }
            break;
	case CMD_SET_INFRA_LED:
            ret = __get_user(ioval, (int __user *)val);
            pr_debug(" infra = %d\n", ioval);
            p = iogrp_find_name("infra");
            if(p)
                iogrp_force_output(p, !!ioval);
	    break;
	case CMD_GET_INFRA_LED:
	    p = iogrp_find_name("infra");
	    if(!p) {
		    pr_err("cannot found io infra\n");
		    return -1;
	    }
	    ioval = gpio_get_value(p->gpio);
	    __put_user(ioval, (int __user *)vp);
	    break;
	case CMD_SET_OIL_CONTROL:
            ret = __get_user(ioval, (int __user *)val);
            pr_debug("read oilcontrol = %d\n", ioval);
	    p = iogrp_find_name("oilc0");
	    pp = iogrp_find_name("oilc1");
	    if(p && pp) {
		    pr_debug("set oil gpio\n");
		    iogrp_force_output(p, ioval & 0xf);
		    iogrp_force_output(pp, (ioval>>4) & 0xf);
	    } else
		    pr_err("p or pp failed.\n");

	    break;
	case CMD_GET_OIL_CONTROL:
	    p = iogrp_find_name("oilc0");
	    pp = iogrp_find_name("oilc1");
	    if(!p || !pp) {
		    pr_err("cannot found io oilc0 or oilc1\n");
		    return -1;
	    }
	    pio = gpio_get_value(p->gpio);
	    ppio = gpio_get_value(pp->gpio);
	    {
		    int ioc = ((!!ppio) << 4) | (!!pio);
		    pr_debug("CMD_GET_OIL_CONTROL = 0x%x\n", ioc);
		    __put_user(ioc, (int __user *)vp);
	    }
	    break;
	case CMD_SET_UART_CONTROL:
            ret = __get_user(ioval, (int __user *)val);
            pr_debug("read uart2sw = %d\n", ioval);
            p = iogrp_find_name("uart2sw");
            if(p)
                iogrp_force_output(p, !!ioval);

	    break;
        default:
            pr_err("ioctl failed, unkown command = %d\n", cmd);
    }

    return 0;
}

static struct file_operations iogrp_ops = {
    .open = iogrp_open,
    .unlocked_ioctl = iogrp_ioctl,
};

static void iogrp_green_led_detect(unsigned long arg)
{
    int ssw;

    ssw = iogrp_get_carstate_led();
    //pr_debug("ssw = %d, carstate = %d\n", ssw, iodev->carstate);
    if(ssw != iodev->carstate) {
        iogrp_set_carstate_led(iodev->carstate);
        mod_timer(&iodev->s_timer, jiffies + HZ + HZ + HZ + HZ);
    } else
        mod_timer(&iodev->s_timer, jiffies + HZ + HZ);
    if(!(iodev->timer_flag--))
        del_timer(&iodev->s_timer);
}

static int iogrp_probe(struct platform_device *pdev)
{
    struct device_node *node;
    struct device *dev = &pdev->dev;
    int error;
    dev_t devno;
    int i;
    struct cdev *cdev;
    struct class *cls;

    pr_info("iogrp_probe ...\n");

    leds_ctrl = gpio_keys_leds_ctrl;

    iodev = kzalloc(sizeof(struct iogrp_dev), GFP_KERNEL);
    if(!iodev) {
        pr_err("kzalloc failed.\n");
        error = - ENOMEM;
        goto err_out;
    }

    iodev->carstate = KERN_CAR_STATE_STOP;
    iodev->timer_flag = 20;

    node = pdev->dev.of_node;
    iodev->node = node;

    error = iogrp_prase_dt(node);
    if(error < 0) {
        pr_err("prase dt failed.\n");
        goto err_free_malloc;
    }

    iodev->pinctrl = devm_pinctrl_get(dev);
    if(IS_ERR(iodev->pinctrl)) {
	    if(PTR_ERR(iodev->pinctrl) == - EPROBE_DEFER)
		    return - EPROBE_DEFER;
	    pr_warn("target does not use pinctrl\n");
	    iodev->pinctrl = NULL;
    }

    if(iodev->pinctrl) {
	    error = iogr_pinctrl_config(1);
	    if(error < 0) {
		    pr_err("set pinctrl active failed.\n");
		    goto err_pinctrl_cfg;
	    }
    }

	for(i = 0; i < iodev->lsize; i++) {
		struct iogrp_desc *io;
		io = iodev->iolist + i;
		if(gpio_is_valid(io->gpio)) {
			error = gpio_request(io->gpio, io->label);
			if(error) {
				pr_err("io %s gpio_request failed\n", io->label);
				goto err_free_gpio;
			}
			error = gpio_direction_output(io->gpio, io->value);
			if(error)
				pr_warn("gpio_direction_output failed, %s -> %d\n", io->label, io->value);
		}
	}

    error = alloc_chrdev_region(&devno, 0, 1, IOGRP_NAME);
    if(error < 0) {
        pr_err("alloc_chrdev_region failed.\n");
        goto err_free_gpio;
    }
    iodev->devno = devno;

    cdev = cdev_alloc();
    if(!cdev) {
        error = - ENOMEM;
        pr_err("cdev_alloc failed.\n");
        goto err_free_devno;
    }
    cdev->owner = THIS_MODULE;
    cdev->ops = &iogrp_ops;
    kobject_set_name(&cdev->kobj, IOGRP_NAME);
    error = cdev_add(cdev, devno, 1);
    if(error < 0) {
        pr_err("cdev_add failed.\n");
        goto err_free_cdev;
    }
    iodev->cdev = cdev;

    cls = class_create(THIS_MODULE, IOGRP_NAME"_cls");
    if(IS_ERR(cls)) {
        pr_err("class_create failed.\n");
        error = PTR_ERR(cls);
        goto err_cdev_del;
    }
    iodev->cls = cls;

    dev = device_create(cls, NULL, devno, NULL, IOGRP_NAME);
    if(IS_ERR(dev)) {
        pr_err("device_create failed.\n");
        error = PTR_ERR(dev);
        goto err_free_cls;
    }
    iodev->dev = dev;

    init_timer(&iodev->s_timer);
    iodev->s_timer.function = iogrp_green_led_detect;
    iodev->s_timer.expires = jiffies + HZ;
    add_timer(&iodev->s_timer);

    error = sysfs_create_group(&pdev->dev.kobj, &keys_leds_attr_group);
    if(error) {
        pr_err("sysfs_create_group failed.\n");
    }

    iogrp_set_carstate_led(KERN_CAR_STATE_STOP);

    return 0;
err_free_cls:
    class_destroy(cls);
err_cdev_del:
    cdev_del(cdev);
err_free_cdev:
    kobject_put(&cdev->kobj);
err_free_devno:
    unregister_chrdev_region(devno, 1);
err_free_gpio:
    for(; i >= 0; i--)
        gpio_free(iodev->iolist[i].gpio);
err_pinctrl_cfg:
    kfree(iodev->iolist);
err_free_malloc:
    kfree(iodev);
err_out:
    return error;
}

static int iogrp_remove(struct platform_device *pdev)
{
	int i;

	pr_info("iogrp_remove ...\n");
	sysfs_remove_group(&pdev->dev.kobj, &keys_leds_attr_group);
	class_destroy(iodev->cls);
	cdev_del(iodev->cdev);
	kobject_put(&iodev->cdev->kobj);
	unregister_chrdev_region(iodev->devno, 1);
	for(i = 0; i < iodev->lsize; i++) {
		int gpio = iodev->iolist[i].gpio;
		if(gpio_is_valid(gpio))
			gpio_free(gpio);
	}
	kfree(iodev->iolist);
	kfree(iodev);
	return 0;
}

static int iogrp_suspend(struct device *dev)
{
	int i;
	struct iogrp_desc *io;

	dev_info(dev, "iogr suspend.\n");
	for(i = 0; i < iodev->lsize; i++) {
		io = iodev->iolist + i;
		io->value = gpio_get_value(io->gpio);
		if(io->suspend_state == IOGRP_SUSPEND_STATE_KEEP) {
			dev_dbg(dev, "%s keep", io->label);
			continue;
		}
		if(gpio_direction_input(io->gpio))
			pr_warn("set %s input failed\n", io->label);
	}
	if(iodev->pinctrl)
		iogr_pinctrl_config(0);
	return 0;
}

static int iogrp_resume(struct device *dev)
{
	int i;
	struct iogrp_desc *io;

	dev_info(dev, "iogr resume.\n");
	if(iodev->pinctrl)
		iogr_pinctrl_config(1);
	for(i = 0; i < iodev->lsize; i++) {
		io = iodev->iolist + i;
		if(io->suspend_state == IOGRP_SUSPEND_STATE_KEEP) {
			dev_dbg(dev, "%s keep value", io->label);
			continue;
		}
		if(gpio_direction_output(io->gpio, io->value))
			pr_warn("set %s = %d failed.\n", io->label, io->value);
	}
	return 0;
}

static SIMPLE_DEV_PM_OPS(iogrp_pm_ops, iogrp_suspend, iogrp_resume);

static struct of_device_id iogrp_of_match[] = {
    { .compatible = "iogrp", },
    { },
};

static struct platform_driver gpio_debug_driver = {
    .probe = iogrp_probe,
    .remove = iogrp_remove,
    .driver = {
        .name = "iogrp",
        .owner = THIS_MODULE,
	.pm = &iogrp_pm_ops,
        .of_match_table = of_match_ptr(iogrp_of_match),
    }
};


static int __init iogrp_init(void)
{
    pr_info("iogrp_init ...\n");

    platform_driver_register(&gpio_debug_driver);

    return 0;
}

static void __exit gpio_debug_exit(void)
{
    printk("gpio_debug_exit .. \n");
    platform_driver_unregister(&gpio_debug_driver);
}

module_init(iogrp_init);
module_exit(gpio_debug_exit);
MODULE_LICENSE("GPL");


#include <linux/module.h>
#include <linux/platform_device.h>

struct s_swev {
	struct input_dev *input;
};

static int sw_ev_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct s_swev *pdata = dev_get_platdata(dev);
	struct input_dev *input;
	int rc;

	dev_info(dev, "%s, in\n", __func__);

	if(!pdata) {
		pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
		if(IS_ERR(pdata))
			return PTR_ERR(pdata);
	}

	input = input_allocate_device();
	if(!input) {
		dev_err(dev, "nomem\n");
		rc = -ENOMEM;
		goto fail1;
	}

	input->name = "swev_test";
	input->dev.parent = &pdev->dev;
	input->open = swev_open;
	input->close = swev_close;

	input_set_capability(input, EV_SW, 109);

	platform_set_drvdata(pdev, pdata);
	input_set_drvdata(input, pdata);

	rc = input_register_device(input);
	if(rc) {
		dev_err(dev, "register fail\n");
		goto fail2;
	}

	return 0;

fail2:
	input_free_device(input);
fail1:
	kfree(pdata);
	return rc;
}

static int sw_ev_remove(struct platform_device *pdev)
{
	pr_info("%s, in\n", __func__);

	return 0;
}

static void    swev_release(struct device *dev)
{
	pr_info("%s, in\n", __func__);
}

static struct platform_device pdevice = {
	.name = "sw_ev_demo",

	.dev = {
		.release = swev_release,
	},
};

static struct platform_driver pdriver = {
	.probe = sw_ev_probe,
	.remove = sw_ev_remove,

	.driver = {
		.name = "sw_ev_demo",
	},
};

static int __init ev_sw_init(void)
{
	pr_info("%s, in\n", __func__);

	platform_device_register(&pdevice);
	platform_driver_register(&pdriver);

	return 0;
}

static void __exit ev_sw_exit(void)
{
	pr_info("%s, in\n", __func__);

	platform_device_unregister(&pdevice);
	platform_driver_unregister(&pdriver);
}

module_init(ev_sw_init);
module_exit(ev_sw_exit);
MODULE_LICENSE("GPL");


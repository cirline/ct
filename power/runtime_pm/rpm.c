#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/delay.h>

static int rpm_suspend(struct device *dev)
{
	dev_info(dev, "suspend\n");

	msleep(1000);
	dev_info(dev, "suspend finish\n");

	return 0;
}

static int rpm_resume(struct device *dev)
{
	dev_info(dev, "resume\n");

	msleep(1000);
	dev_info(dev, "resume finish\n");

	return 0;
}

static int rpm_idle(struct device *dev)
{
	dev_info(dev, "idle\n");

	msleep(1000);
	dev_info(dev, "idle finish\n");

	return 0;
}

static void rpm_release(struct device *dev)
{
	dev_info(dev, "release\n");
}

static ssize_t rpm_show(struct device *dev,struct device_attribute *attr,
		char *buf)
{
	dev_info(dev, "show\n");

	return 0;
}

static ssize_t rpm_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	dev_info(dev, "store, buf: %s, count=%lu\n", buf, count);

	if(*buf == 'y') {
		dev_info(dev, "get start\n");
		pm_runtime_get_sync(dev);
		dev_info(dev, "get end\n");
	} else {
		dev_info(dev, "put start\n");
		pm_runtime_put_sync(dev);
		dev_info(dev, "put end\n");
	}

	return count;
}

static DEVICE_ATTR(rpm, S_IRUGO | S_IWUSR, rpm_show, rpm_store);

static const struct dev_pm_ops rpm_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(rpm_suspend, rpm_resume)
	SET_RUNTIME_PM_OPS(rpm_suspend, rpm_resume, rpm_idle)
};

static int rpm_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int rc;

	dev_info(dev, "probe\n");

	rc = device_create_file(dev, &dev_attr_rpm);
	if(rc < 0) {
		dev_err(dev, "device create file failed\n");
		goto err_dcf;
	}

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);

	return 0;

err_dcf:
	return rc;
}

static int rpm_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	dev_info(dev, "remove\n");

	device_remove_file(dev, &dev_attr_rpm);

	pm_runtime_disable(dev);

	return 0;
}

static struct platform_driver rpm_driver = {
	.driver = {
		.name = "rpm_demo",
		.owner = THIS_MODULE,
		.pm = &rpm_pm_ops,
	},
	.probe = rpm_probe,
	.remove = rpm_remove,
};


static struct platform_device rpm_device = {
	.name = "rpm_demo",
	.dev = {
		.release = rpm_release,
	},
};

static int __init rpm_init(void)
{
	pr_info("rpm_init\n");
	platform_device_register(&rpm_device);
	platform_driver_register(&rpm_driver);

	return 0;
}

static void __exit rpm_exit(void)
{
	pr_info("rpm_exit\n");
	platform_driver_unregister(&rpm_driver);
	platform_device_unregister(&rpm_device);
}

module_init(rpm_init);
module_exit(rpm_exit);
MODULE_LICENSE("GPL");


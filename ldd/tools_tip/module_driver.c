#include <linux/module.h>
#include <linux/device.h>

int int_driver = 89;

static int m_register(int *drv)
{
	pr_info("%s, in\n", __func__);

	pr_info("int_driver = %d\n", *drv);

	*drv = 1919;

	return 0;
}

static void m_unregister(int *drv)
{
	pr_info("%s, in\n", __func__);
	pr_info("int_driver = %d\n", *drv);
}

module_driver(int_driver, m_register, m_unregister);

MODULE_LICENSE("GPL");


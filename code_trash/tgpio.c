#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>
#include <mach/sys_config.h>
#include <linux/pinctrl/pinconf-sunxi.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinconf.h>
#include <asm-generic/gpio.h>
#include <mach/gpio.h>

static int t8_rst = 0;

static int t8_gpio_requ(struct gpio_config *gpio)
{
	int            ret = 0;
	char           pin_name[8] = {0};
	unsigned long  config;

	sunxi_gpio_to_name(gpio->gpio, pin_name);
	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, gpio->mul_sel);
	ret = pin_config_set(SUNXI_PINCTRL, pin_name, config);
	if (ret) {
		printk("set gpio %s mulsel failed.\n",pin_name);
		return -1; 
	}
	if (gpio->pull != GPIO_PULL_DEFAULT){
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, gpio->pull);
		ret = pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (ret) {
			printk("set gpio %s pull mode failed.\n",pin_name);
			return -1; 
		}
	} 
	if (gpio->drv_level != GPIO_DRVLVL_DEFAULT){
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DRV, gpio->drv_level);
		ret = pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (ret) {
			printk("set gpio %s driver level failed.\n",pin_name);
			return -1; 
		}
	} 
	if (gpio->data != GPIO_DATA_DEFAULT) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, gpio->data);
		ret = pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (ret) {
			printk("set gpio %s initial val failed.\n",pin_name);
			return -1; 
		}
	} 
	return 0;
}

void tgg_pulldown(void)
{
	__gpio_set_value(t8_rst, 0);
}
EXPORT_SYMBOL(tgg_pulldown);
void tgg_pullup(void)
{
	__gpio_set_value(t8_rst, 1);
}
EXPORT_SYMBOL(tgg_pullup);


#define PE14_CON	0x01C20894
#define CON_OFFSET	24
#define PE14_DATA	0x01C208A0
#define DATA_OFFSET	14

#define N2A(n)		((unsigned int volatile *)(n))
unsigned int rrval, rwval;
unsigned int drval, dwval;

unsigned int vir_addr = 0;


static __init int tgg_init(void)
{
#if 0
	script_item_u t8_val;
	script_item_value_type_e t8_type;
	struct gpio_config *gpio_p = NULL;

	t8_type = script_get_item("csi0", "vip_dev0_reset", &t8_val);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO!=t8_type) {
		printk("get vip_dev0_reset error\n");
	} else {
		gpio_p = &t8_val.gpio;
		t8_rst = gpio_p->gpio;
		t8_gpio_requ(gpio_p);
	}

//			gpio_direction_output(t8_rst, 0);
#endif

	vir_addr = ioremap(0x01C20800, SZ_1K);

	printk("vir addr = 0x%x\n", vir_addr);

	drval = __raw_readl(vir_addr + 0xA0);
	rrval = __raw_readl(vir_addr + 0x94);

	printk("drval = 0x%x, rrval = 0x%x\n", drval, rrval);

	rwval = (rrval & (~(0xf<<CON_OFFSET))) | (1<<CON_OFFSET);
	dwval = (drval & (~(0x1<<DATA_OFFSET))) | (0<<CON_OFFSET);

	__raw_writel(rwval, vir_addr + 0x94);
	__raw_writel(dwval, vir_addr + 0xA0);

	printk("dwval = 0x%x, rwval = 0x%x\n", dwval, rwval);

	printk("hell module \n");
	return 0;
}

static __exit void tgg_exit(void)
{
//	__gpio_set_value(t8_rst, 1);
	//gpio_free(t8_rst);
	//
	__raw_writel(rrval, vir_addr + 0x94);
	__raw_writel(drval, vir_addr + 0xA0);
	printk("module exit. \n");
}




module_init(tgg_init);
module_exit(tgg_exit);
MODULE_LICENSE("GPL");

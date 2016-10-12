/**
 * shell function type: int (*)(void *)
 */

#include <common.h>
#include <timer.h>
#include <irq.h>
#include <display.h>

int shell_dump_registers(void *p)
{
	pr_info("%s\n", __func__);

	dump_stack_frame();
	__asm__ __volatile__("push {r0-r4}" : :);
	dump_stack_frame();
	__asm__ __volatile__("pop {r0-r4}" : :);
	dump_stack_frame();

	pr_info("dump_registers\n");
	dump_registers();
	dump_stack_frame();

	return 0;
}

int timer_1hz_buzz_isr(void)
{
	pr_info("%s\n", __func__);

	irq_clear_pending(TIMERINT(1));

	return 0;
}

int shell_timer_1hz_buzz(void *p)
{
        struct timer timer;
        timer_default_cfg(&timer);
        timer.sn = TIMER1;
        timer.auto_reload = TIMER_INTERVAL;
	timer.irq_enable = 1;
	irq_init(TIMERINT(1), timer_1hz_buzz_isr);

        timer_init(&timer);
        timer_toggle(timer.sn, 1);

	return 0;
}

int shell_sleep_test(void *p)
{
	int i = 0;

	pr_info("%s --> %x\n", __func__, i++);
	mdelay(1000);
	pr_info("%s --> %x\n", __func__, i++);
	mdelay(2000);
	pr_info("%s --> %x\n", __func__, i++);
	mdelay(3000);
	pr_info("%s --> %x\n", __func__, i++);
	mdelay(4000);

	return 0;
}

int shell_lcd_test(void)
{
#if 0
	struct hv_config at070tn92_lcd = {
		.hozval = 800,
		.linehoz = 1056,
		.hspw = 10,
		.hbpd = 46,
		.hfpd = 210,
		.lineval = 480,
		.frameline = 525,
		.vspw = 10,
		.vbpd = 23,
		.vfpd = 22,
		.dclk = 33,
	};
#else
	struct hv_config at070tn92_lcd = {
		.hozval = 5,
		.linehoz = 12,
		.hspw = 2,
		.hbpd = 3,
		.hfpd = 4,
		.lineval = 4,
		.frameline = 9,
		.vspw = 3,
		.vbpd = 2,
		.vfpd = 3,
		.dclk = 33,
	};
#endif

	generate_hv_signal(&at070tn92_lcd);

	return 0;
}


int timer2_flashleds_isr(void)
{
	static int count = 0;

	//pr_info("%s\n", __func__);
	irq_clear_pending(TIMERINT(2));

	gpio_set_value('c', 0, 3, count & 1);
	count++;

	return 0;
}

int shell_flashleds_test(void)
{
        struct timer timer;
        timer_default_cfg(&timer);
	timer_setcfg_period(&timer, 2000);
        timer.sn = TIMER2;
        timer.auto_reload = TIMER_INTERVAL;
	timer.irq_enable = 1;
	irq_init(TIMERINT(2), timer2_flashleds_isr);

        timer_init(&timer);
        timer_toggle(timer.sn, 1);

	return 0;
}


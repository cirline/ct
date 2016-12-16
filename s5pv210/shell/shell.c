/**
 * shell function type: int (*)(void *)
 */

#include "utils.h"
#include <timer.h>
#include <irq.h>
#include <display.h>
#include "clock.h"
#include "list.h"
#include "shell.h"

int shell_dump_registers(int argc, char *argv[])
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

int timer_1hz_buzz_isr(int argc, char *argv[])
{
	pr_info("%s\n", __func__);

	irq_clear_pending(TIMERINT(1));

	return 0;
}

int shell_timer_1hz_buzz(int argc, char *argv[])
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

int shell_sleep_test(int argc, char *argv[])
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

int shell_lcd_test(int argc, char *argv[])
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


int timer2_flashleds_isr(int argc, char *argv[])
{
	static int count = 0;

	//pr_info("%s\n", __func__);
	irq_clear_pending(TIMERINT(2));

	gpio_set_value('c', 0, 3, count & 1);
	count++;

	return 0;
}

int shell_flashleds_test(int argc, char *argv[])
{
        struct timer timer;
        timer_default_cfg(&timer);
	timer_setcfg_period(&timer, 200);
        timer.sn = TIMER2;
        timer.auto_reload = TIMER_INTERVAL;
	timer.irq_enable = 1;
	irq_init(TIMERINT(2), timer2_flashleds_isr);

        timer_init(&timer);
        timer_toggle(timer.sn, 1);

	return 0;
}

int shell_dump_sfrs(int argc, char *argv[])
{
	unsigned long regs[] = {
		CLK_SRC(0),
		CLK_DIV(0),
	};
	int i;
	unsigned long regval;

	for(i = 0; i < ARRAY_SIZE(regs); i++) {
		regval = __raw_read(regs[i]);

		pr_info("%x - %x\n", regs[i], regval);
	}

	clock_reset();

	return 0;
}

int shell_devmem(int argc, char *argv[])
{
	unsigned long regaddr, regval;

	if(argc < 2) {
		pr_err("eg: devmem reg_addr [reg_value]\n");
		return -1;
	}
	regaddr = str2hl(argv[1]);
	if(regaddr == -1) {
		pr_err("error register address: %s\n", argv[1]);
		return -1;
	}
	if(argc == 2) {
		/* read register */
		regval = __raw_read(regaddr);
		printf("r %x \t= %x\n", regaddr, regval);
		return 0;
	}

	regval = str2hl(argv[2]);
	if(regval == -1) {
		pr_err("error register value: %s\n", argv[2]);
		return -1;
	}
	/* write register */
	__raw_write(regaddr, regval);
	printf("w %x \t= %x\n", regaddr, regval);

	return 0;
}

int shell_parse_arguments(char *str, char *argv[SHELL_MAX_ARGUMENTS])
{
	int i, j;

	for(i = 0; *str; i++) {
		argv[i] = str;
		while(*str != ' ' && *str != '\t' && *str != 0)
			str++;
		if(*str) {
			*str = 0;
			str++;
		}
	}

	pr_info("argc = %x\n", i);
	for(j = 0; j < i; j++) {
		pr_info("argv[%x] = %s\n", j, argv[j]);
	}

	return i;
}


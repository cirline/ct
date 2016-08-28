/**
 * shell function type: int (*)(void *)
 */

#include <common.h>
#include <timer.h>
#include <irq.h>

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


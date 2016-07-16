#include "timer.h"

int timer_1hz_buzz_test(void *p)
{
        struct timer timer;
        timer_default_cfg(&timer);
        timer.sn = TIMER1;
        timer.auto_reload = TIMER_INTERVAL;
        timer_init(&timer);
        timer_toggle(timer.sn, 1);

	return 0;
}


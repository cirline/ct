#include "keyboard.h"
#include "common.h"
#include "timer.h"

/* key press interrupt function */
int keydown(void)
{
	struct timer timer;
	fstart();
	timer_default_cfg(&timer);
	timer.sn = TIMER1;
	timer_init(&timer);
	timer_toggle(timer.sn, 1);
	eint_pending(0, 1);

	return 0;
}


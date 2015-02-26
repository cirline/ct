#include "keyboard.h"
#include "common.h"
#include "timer.h"

/* key press interrupt function */
int keydown(void)
{
	fstart();
	timer_init(TIMER1, 16110, 8550);
	timer_toggle(TIMER1, 1);
	eint_pending(0, 1);

	return 0;
}


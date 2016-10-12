#include "../timer.c"

void inline timer_setcfg_period(struct timer *timer, int ms)
{
	timer->count_buffer =  ms * 16;
	timer->cmp_buffer = ms * 8;
}

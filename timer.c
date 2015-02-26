#include "common.h"
#include "s5p_regs.h"
#include "timer.h"

void inline timer_update(enum timer_sn t_sn)
{
	region_write(TCON, 0x1, (t_sn == TIMER0) ? 1 : ((t_sn<<2) + 5), 1);
}

void timer_toggle(enum timer_sn t_sn, int x)
{
	region_write(TCON, 0x1, (t_sn ==  TIMER0) ? 1 : ((t_sn<<2) + 5), 0);
	region_write(TCON, 0x1, (t_sn ==  TIMER0) ? 0 : ((t_sn<<2) + 4), (x & 1));
}

/**
 * t_sn: timer sn
 * cnt: count buffer, down-counter load initial number from cnt
 * high_ratio: TCMPBn register, pwm high level region 0 < high_ratio < cnt
 ** frequency
 * f(timer input clock) = PCLK / (prescaler+1) / divider
 * f = 66MHz / 256 / 16 = 16113 Hz
 */
int timer_init(enum timer_sn t_sn, int cnt, int high_ratio)
{
	/* set tout */
	region_write(GPDxCON(0), MASK_BITS_4, (t_sn<<2), 0x2);
	/* set prescaler */
	region_write(TCFG0, MASK_BITS_16, 0, TIMER_PRESCALER);
	/* set divider */
	region_write(TCFG1, MASK_BITS_4, (t_sn<<2), TIMER_DIVIDER16);
	/* set timer */
#define SET_TIMER_TCON	(TIMER_AUTO_RELOAD_ON)
	region_write(TCON, MASK_BITS_4, (t_sn == TIMER0) ? 0 : ((t_sn<<2) + 4), SET_TIMER_TCON);
	/* set period */
	__raw_write(TCNTBx(t_sn), cnt);
	__raw_write(TCMPBx(t_sn), high_ratio);
	timer_update(t_sn);

	return 0;
}


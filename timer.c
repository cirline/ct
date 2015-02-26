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

int timer_init(enum timer_sn t_sn, int cnt, int lev)
{
	/* set tout */
	region_write(GPDxCON(0), 0xf, (t_sn<<2), 0x2);
	/* set prescaler */
	region_write(TCFG0, 0xffff, 0, 0xffff);
	/* set divider */
	region_write(TCFG1, 0xf, (t_sn<<2), 0x4);
	/* set timer */
	region_write(TCON, 0xf, (t_sn == TIMER0) ? 0 : ((t_sn<<2) + 4), (1<<3)|(1<<2));
	/* set period */
//	__raw_write(TCNTBx(tn), 16110);
//	__raw_write(TCMPBx(tn), 8550);
	__raw_write(TCNTBx(t_sn), cnt);
	__raw_write(TCMPBx(t_sn), lev);
	timer_update(t_sn);

	return 0;
}


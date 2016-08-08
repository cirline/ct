#include "common.h"
#include "s5p_regs.h"
#include "timer.h"

/**
 * update TCNTBn & TCMPBn to down-counter
 */
void inline timer_update(enum timer_sn t_sn)
{
	region_write(TCON, MASK_BITS_1, TIMER_MANUAL_UPDATE + ((t_sn == TIMER0) ? 0 : ((t_sn+1)<<2)), 1);
}

void inline timer_toggle(enum timer_sn t_sn, int x)
{
	region_write(TCON, 0x1, (t_sn ==  TIMER0) ? 1 : ((t_sn<<2) + 5), 0);
	region_write(TCON, 0x1, (t_sn ==  TIMER0) ? 0 : ((t_sn<<2) + 4), (x & 1));
}

/**
 * cnt & cmp see timer_init()
 * if cnt == NULL, function will use the old cnt in TCNTBn
 */
void timer_set_period(enum timer_sn t_sn, int cnt, int cmp)
{
	if(cnt) {
		__raw_write(TCNTBx(t_sn), cnt);
		__raw_write(TCMPBx(t_sn), cmp);
	} else {
		__raw_write(TCMPBx(t_sn), cmp & __raw_read(TCNTBx(t_sn)));
	}
}

/**
 * timer_init - support timer0 ~ timer3 init
 * @pt: timer config
 *
 * t_sn: timer sn
 * cnt: count buffer, down-counter load initial number from cnt
 * cmp: TCMPBn register, pwm high level region 0 < cmp < cnt, in other words, pwm will invert when cnt == cmp
 ** frequency
 * f(timer input clock) = PCLK / (prescaler+1) / divider
 * f = 66MHz / 256 / 16 = 16113 Hz
 */
int timer_init(struct timer *pt)
{
	/* set tout */
	region_write(GPDxCON(0), MASK_BITS_4, (pt->sn<<2), 0x2);
	/* set prescaler */
	region_write(TCFG0, MASK_BITS_8, ((pt->sn > TIMER1) ? 8 : 0), pt->prescaler);
	/* set divider */
	region_write(TCFG1, MASK_BITS_4, (pt->sn<<2), pt->divider);
	/* set timer */
	region_write(TCON, MASK_BITS_4, (pt->sn == TIMER0) ? 0 : ((pt->sn<<2) + 4), \
            pt->auto_reload<<TIMER_AUTO_RELOAD | \
            pt->out_invert<<TIMER_TOUT_INVERTER );
	/* set period */
	timer_set_period(pt->sn, pt->count_buffer, pt->cmp_buffer);
	timer_update(pt->sn);
	timer_irq_toggle(pt->sn, pt->irq_enable);

	return 0;
}

void timer_default_cfg(struct timer *pt)
{
    pt->sn = TIMER0;
    pt->count_buffer = TIMER_DEF_CNTB;
    pt->cmp_buffer = TIMER_DEF_CNTB / 2;    /* duty ratio is 50% */
    pt->prescaler = 0xFF;
    pt->divider = TIMER_DIVIDER16;
    pt->auto_reload = TIMER_ONESHOT;
    pt->out_invert = TIMER_INVERT_OFF;
	pt->irq_enable = 0;
}

void inline timer_irq_toggle(enum timer_sn t_sn, int enable)
{
	region_write(TINT_CSTAT, MASK_BITS_1, t_sn, enable);
}

int inline timer_irq_status(enum timer_sn t_sn)
{
	return region_read(TINT_CSTAT, MASK_BITS_1, t_sn+5);
}

int inline timer_cnt_status(enum timer_sn t_sn)
{
	return __raw_read(TCNTOx(t_sn));
}

void inline timer_irq_clear(enum timer_sn t_sn)
{
	set2clear(TINT_CSTAT, MASK_BITS_1, t_sn+5);
}

int timer_spin_lock(enum timer_sn t_sn, int ms)
{
    struct timer timer;
	int n;

    timer_default_cfg(&timer);
    timer.sn = t_sn;
    timer.count_buffer =  ms * 16;
    timer.cmp_buffer = ms * 8;
	timer.irq_enable = 1;
    timer_init(&timer);
    timer_toggle(timer.sn, 1);
	do {
		n = timer_irq_status(timer.sn);
	} while(!n/*timer_irq_status(timer.sn)*/);
	timer_irq_clear(timer.sn);

	return 0;
}


#ifndef __TIMER_H__
#define __TIMER_H__

/* timer serial number */
enum timer_sn { TIMER0 = 0, TIMER1, TIMER2, TIMER3, TIMER_NONE};

/* prescaler: 0 ~ 0xFF */
#define TIMER_PRESCALER		((0xff<<8) | 0xff)

/* divider */
enum timer_divider {
	TIMER_DIVIDER1 = 0,
	TIMER_DIVIDER2,
	TIMER_DIVIDER4,
	TIMER_DIVIDER8,
	TIMER_DIVIDER16,
	TIMER_DIVIDER_SCLK_PWM,
};

enum timer_tcon {
	TIMER_START = 1<<0,
	TIMER_MANUAL_UPDATE = 1<<1,
	TIMER_TOUT_INVERTER_ON = 1<<2,
	TIMER_AUTO_RELOAD_ON = 1<<3,
	TIMER_DEAD_ZONE = 1<<4,
};

extern void inline timer_update(enum timer_sn t_sn);
extern void timer_toggle(enum timer_sn t_sn, int x);
extern void timer_set_period(enum timer_sn t_sn, int cnt, int high_ratio);
extern int timer_init(enum timer_sn t_sn, int cnt, int high_ratio);

#endif


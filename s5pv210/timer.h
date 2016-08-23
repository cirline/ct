#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_DEF_CNTB	16112

/* timer serial number */
enum timer_sn { TIMER0 = 0, TIMER1, TIMER2, TIMER3, TIMER4, TIMER_NONE};

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
	TIMER_START = 0,
	TIMER_MANUAL_UPDATE = 1,
	TIMER_TOUT_INVERTER = 2,
	TIMER_AUTO_RELOAD = 3,
	TIMER_DEAD_ZONE = 4,
};

enum { TIMER_INVERT_OFF = 0, TIMER_INVERT_ON };
enum { TIMER_ONESHOT = 0, TIMER_INTERVAL };

struct timer {
    /* timer serial number */
    enum timer_sn sn;
    /* down-counter count buffer, timer will intrrupt when zero */
    int count_buffer;
    /* compare register, TOUT will invert when cntb == cmpb */
    int cmp_buffer;     // high_ratio;
    /* prescaler: 0 ~ 0xFF */
    int prescaler;
    enum timer_divider divider;
    int auto_reload:1;
    int out_invert:1;
	int irq_enable:1;
	int irq_flag:1;
};

extern void inline timer_update(enum timer_sn t_sn);
extern void timer_toggle(enum timer_sn t_sn, int x);
extern void timer_set_period(enum timer_sn t_sn, int cnt, int cmp);
extern void timer_default_cfg(struct timer *pt);
extern int timer_init(struct timer *pt);
extern void inline timer_irq_toggle(enum timer_sn t_sn, int enable);
extern int inline timer_irq_status(enum timer_sn t_sn);
extern void inline timer_irq_clear(enum timer_sn t_sn);
extern int timer_spin_lock(enum timer_sn t_sn, int ms);

#endif


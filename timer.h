#ifndef __TIMER_H__
#define __TIMER_H__

/* timer serial number */
enum timer_sn { TIMER0 = 0, TIMER1, TIMER2, TIMER3 };

extern void inline timer_update(enum timer_sn t_sn);
extern void timer_toggle(enum timer_sn t_sn, int x);
extern int timer_init(enum timer_sn t_sn, int cnt, int lev);

#endif


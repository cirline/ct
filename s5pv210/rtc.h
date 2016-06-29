#ifndef __RTC_H__
#define __RTC_H__

typedef struct {
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
} rtc_t;

int rtc_init(void);
extern int rtc_set(rtc_t *pr);
extern int rtc_set_alarm(rtc_t *pr);
extern void rtc_clear_int_pending(void);
void rtc_print(void);

#endif


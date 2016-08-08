#include "s5p_regs.h"
#include "common.h"
#include "rtc.h"

int rtc_init(void)
{
	__raw_write(RTCCON, 0x1);
	return 0;
}

int rtc_set(rtc_t *pr)
{
	__raw_write(BCDSEC, pr->sec);
	__raw_write(BCDMIN, pr->min);
	__raw_write(BCDHOUR, pr->hour);
	__raw_write(BCDDAY, pr->day);
	__raw_write(BCDMON, pr->mon);
	__raw_write(BCDYEAR, pr->year);

	return 0;
}

int rtc_set_alarm(rtc_t *pr)
{
	__raw_write(ALMSEC, pr->sec);
	__raw_write(ALMMIN, pr->min);
	__raw_write(ALMHOUR, pr->hour);
	__raw_write(ALMDAY, pr->day);
	__raw_write(ALMMON, pr->mon);
	__raw_write(ALMYEAR, pr->year);
	__raw_write(RTCALM, 0x7f);
	return 0;
}

void rtc_clear_int_pending(void)
{
	set2clear(INTP, 0x1, 1);
}

void rtc_print(void)
{
	printf("rtc: %p-%p-%p %p:%p:%p\n",
			__raw_read(BCDYEAR),
			__raw_read(BCDMON),
			__raw_read(BCDDAY),
			__raw_read(BCDHOUR),
			__raw_read(BCDMIN),
			__raw_read(BCDSEC));
}

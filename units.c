//#include <config/autoconf.h>
//#include <mach/gpio.h>

#define	GPC0CON	(*(volatile unsigned long *)0xE0200060)
#define	GPC0DAT	(*(volatile unsigned long *)0xE0200064)


#define	GPH0CON	(*(volatile unsigned long *)0xE0200C00)
#define	GPH0DAT	(*(volatile unsigned long *)0xE0200C04)

#define	GPD0CON	(*(volatile unsigned long *)0xE02000A0)
#define	GPD0DAT	(*(volatile unsigned long *)0xE02000A4)

static inline void led_init(void)
{
	//1. 设置GPC0_3,4引脚功能:	输出功能
	GPC0CON &= ~((0xf<<12)|(0xf<<16));
	GPC0CON |= (0x1<<12)|(0x1<<16);
}

static inline void led3if(int val)
{
	GPC0DAT &= ~(0x1<<3);
	GPC0DAT |= ((val & 0x1)<<3);
}

static inline void led4if(int val)
{
	GPC0DAT &= ~(0x1<<4);
	GPC0DAT |= ((val & 0x1)<<4);
}

#define DEF_DELAY 1000000
#define DELAY ((DEF_DELAY / 1024 ) << 5)

static inline void sleep(int n)
{
	if(n <= 0)
		n = DEF_DELAY;
	while(n--);
}

/* mode=1: 同步闪烁 */
static void flash(int n, int delay, int mode)
{
	if(n <=0)
		n = 8;
	while(n--) {
		sleep(delay);
		led3if(1);
		led4if(mode);
		sleep(delay);
		led3if(0);
		led4if(!mode);
	}
}

int main(void)
{
	unsigned long n;
	int val;

	led_init();

	while(1) {
		flash(8, DELAY, 0);
		flash(8, DEF_DELAY, 1);
		continue;

		val = *(volatile unsigned long *)0xE0000000;
		n = 32;
		while(n--) {
			sleep(0);
			led3if(1);
			led4if(val & 0x1);
			sleep(0);
			led3if(0);
			val >>= 1;
		}
	}

	return 0;
}



#include <config/autoconf.h>
#include <mach/gpio.h>

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

static inline void sleep(void)
{
	int i;
	for(i=0;i<1000000;i++);
}

int main(void)
{
	unsigned long i, n;
	int val;

	led_init();

	while(1) {
		n = 0x8;
		while(n--)
		{
			for(i=0;i<1000000;i++);
			led3if(1);
			led4if(1);
			for(i=0;i<1000000;i++);
			led3if(0);
			led4if(0);
		}

		val = *(volatile unsigned long *)0xE0000000;
		n = 32;
		while(n--) {
			sleep();
			led3if(1);
			led4if(val & 0x1);
			sleep();
			led3if(0);
			val >>= 1;
		}
	}

	return 0;
}



//#include <config/autoconf.h>
//#include <mach/gpio.h>
#include "s5p_regs.h"
#include "uart.h"
#include "common.h"
#include "irq.h"

#define	GPD0CON	(*(volatile unsigned long *)0xE02000A0)
#define	GPD0DAT	(*(volatile unsigned long *)0xE02000A4)

static inline void led_init(void)
{
	//1. 设置GPC0_3,4引脚功能:	输出功能
	int val;
	val = __s5p_read(RGPIO_BASE + GPC0CON) & ~((0xf<<12)|(0xf<<16));
	__s5p_wirte(RGPIO_BASE + GPC0CON, val | (0x1<<12)|(0x1<<16));
}

static inline void led3if(int val)
{
	int regval;
	regval = __s5p_read(RGPIO_BASE + GPC0DAT) & ~(0x1<<3);
	__s5p_wirte(RGPIO_BASE + GPC0DAT, (regval | ((val & 0x1)<<3)));
}

static inline void led4if(int val)
{
	int regval;
	regval = __s5p_read(RGPIO_BASE + GPC0DAT) & ~(0x1<<4);
	__s5p_wirte(RGPIO_BASE + GPC0DAT, (regval | ((val & 0x1)<<4)));
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
	int val=0;

	irq_init(0,0);
	led_init();
	uart_init();

	while(1) {
		cprint("flash loop! --- 0x%p\r\n", val++);
		flash(2, DELAY, 0);
		flash(2, DEF_DELAY, 1);
	}

	return 0;
}



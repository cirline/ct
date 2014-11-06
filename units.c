//#include <config/autoconf.h>
//#include <mach/gpio.h>
#include "s5p_regs.h"
#include "uart.h"
#include "common.h"
#include "irq.h"
#include "nf.h"
#include "timer.h"

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

int key0_func(void)
{
	static int n = 0;
	printf("key0 func --- 0x%p\r\n", n++);

	eint_pending(0, 0);

	return 0;
}

int keydown(void)
{
	printf("%p --- %s \n", __LINE__, __func__);
	timer_init();
	timer_toggle(1);
	eint_pending(0, 1);

	return 0;
}

int keyleft(void)
{
	printf("%p --- %s \n", __LINE__, __func__);
	eint_pending(0, 2);
	return 0;
}

int keyright(void)
{
	printf("%p --- %s \n", __LINE__, __func__);
	eint_pending(0, 3);
	return 0;
}

int keyenter(void)
{
	printf("%p --- %s \n", __LINE__, __func__);
	eint_pending(0, 4);
	return 0;
}

int keyback(void)
{
	printf("%p --- %s \n", __LINE__, __func__);
	eint_pending(0, 5);
	return 0;
}

#define KEYMUXMASK	((1<<6) | (1<<7))
int key_mux(void)
{
	int val = KEYMUXMASK & __raw_read(GPHxDAT(2));
	int pend = KEYMUXMASK & __raw_read(EXT_INT_x_PEND(2));

	printf("%p --- %s --- key value is 0x%p\n", __LINE__, __func__, val);
	printf("%p --- %s --- pend value is 0x%p\n", __LINE__, __func__, pend);
	switch(val) {
		case KEYMUXMASK ^ (1<<6):
			printf("keyhome pressed! \n");
			eint_pending(2, 6);
			break;
		case KEYMUXMASK ^ (1<<7):
			printf("keypower pressed! \n");
			eint_pending(2, 7);
			break;
		default:
			printf("unknown key pressed! \n");
			set2clear(EXT_INT_x_PEND(2), 0xff, 0);
			break;
	}

	pend = KEYMUXMASK & __raw_read(EXT_INT_x_PEND(2));
	printf("%p --- %s --- clear pend value is 0x%p\n", __LINE__, __func__, pend);

	//eint_pending(2, ~val);
	return 0;
}

int timer1_int_func(void)
{
	fstart();
	tint_pending(1);
	return 0;
}


int main(void)
{
	int val=0;
	unsigned char buf[2048];

	irq_init(EINT(0), key0_func);
	irq_init(EINT(1), keydown);
	irq_init(EINT(2), keyleft);
	irq_init(EINT(3), keyright);
	irq_init(EINT(4), keyenter);
	irq_init(EINT(5), keyback);
	irq_init(EINT(22), key_mux);
	irq_init(EINT(23), key_mux);
	led_init();
	uart_init();
	nf_init();
	timer_init();
	irq_init(TIMERINT(1), timer1_int_func);
	
/*
	nf_erase(0);
	nf_read(0, buf);
	for(val=0; val<32; val++) {
		printf("[%p]\t= %p\n", val, buf[val]);
	}
	for(val=0; val<32; val++) {
		buf[val] = val << 1;
	}
	nf_write(0, buf);
	for(val=0; val<32; val++) {
		buf[val] = 0;
	}
	sleep(0);
*/
//	timer_toggle(1);
	nf_read(0, buf);
	for(val=0; val<32; val++) {
		printf("[%p]\t= %p\n", val, buf[val]);
	}

	while(1) {
		printf("main --- loop! ... 0x%p\n", val++);
		flash(2, DELAY, 0);
		flash(2, DEF_DELAY, 1);
	}

	return 0;
}



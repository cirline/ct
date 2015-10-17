//#include <config/autoconf.h>
//#include <mach/gpio.h>
#include "s5p_regs.h"
#include "uart.h"
#include "common.h"
#include "irq.h"
#include "nf.h"
#include "timer.h"
#include "rtc.h"
#include "i2c.h"
#include "lcd.h"
#include "keyboard.h"
#include "task.h"

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

/* mode=1: 同步闪烁 */
static void flash(int n, int ms, int mode)
{
	if(n <=0)
		n = 8;
	while(n--) {
		sleep(ms);
		led3if(1);
		led4if(mode);
		sleep(ms);
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

int rtcalm_int_func(void)
{
	fstart();
	rtc_print();
	rtc_clear_int_pending();
	fend();
	return 0;
}

int uart0_int_func(void)
{
	int n;
	n = __raw_read(URXHx(0)) & 0xff;
//	printf("%s n = 0x%p\n", __func__, n);
//	printf("uintm 1= 0x%p\n", __raw_read(UINTMx(0)));
//	printf("uintsp 1= 0x%p\n", __raw_read(UINTSPx(0)));
//	printf("uintp 1= 0x%p\n", __raw_read(UINTPx(0)));
	uint_pending(0);
//	printf("uintm 2= 0x%p\n", __raw_read(UINTMx(0)));
//	printf("uintsp 2= 0x%p\n", __raw_read(UINTSPx(0)));
//	printf("uintp 2= 0x%p\n", __raw_read(UINTPx(0)));
	fend();

	return 0;
}

int main(void)
{
	int val=0;
	unsigned char buf[2048];
	rtc_t rtc;
	int i;
	int task_loop_run = 0;
    unsigned long taskset;

#if 0
	__raw_write(VICxADDRESS(0), 0);
	__raw_write(VICxADDRESS(1), 0);
	irq_init(EINT(0), key0_func);
	irq_init(EINT(1), keydown);
	irq_init(EINT(2), keyleft);
	irq_init(EINT(3), keyright);
	irq_init(EINT(4), keyenter);
	irq_init(EINT(5), keyback);
	irq_init(EINT(22), key_mux);
	irq_init(EINT(23), key_mux);
	irq_init(RTCALMINT(), rtcalm_int_func);
	led_init();
	nf_init();
	rtc_init();
	i2c_init();
	irq_init(TIMERINT(1), timer1_int_func);
	irq_init(UARTINT(0), uart0_int_func);

//	lcd_init();

	rtc.sec = 0x14;
	rtc.min = 0x13;
	rtc.hour = 0x22;
	rtc.day = 0x10;
	rtc.mon = 0x10;
	rtc.year = 0x989;

	rtc_set(&rtc);

	rtc.min = 0x15;
	rtc_set_alarm(&rtc);
	
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

	val = 0;
#endif
    task_init(&taskset);
	while(1) {
        task_loop(&taskset);
#if 0
//		printf("main --- loop! ... 0x%p\n", val++);
		rtc_print();
//		*buf = getchar();
		buf[1] = '\0';
//		printf("char = %s\n", buf);
#if 0
		for(val=1; val<8; ) {
			i2c_read_byte(val++, buf);
			printf("after i2c read byte = 0x%p\n", (int)buf[0]);
		}
#endif
		for(i = 0; i < 8; i++) {
			val += i;
			buf[32+i] = val;
			printf("write i2c read byte 0x%p = 0x%p\n", i, buf[32+i]);
			i2c_write_byte(i, val);
		}
		buf[32+i] = '\0';
		printf("buf string %s\n", buf + 32);
//		i2c_write_array(0, buf + 32, 8);
		i2c_read_array(0, buf, 8);
		for(i = 0; i < 8; i++) {
			printf("after i2c read byte = 0x%p\n", (int)buf[i]);
		}
		flash(2, 500, 1);
        i = 0xffff;
        while(i--);
        printf("loop count = 0x%x\n", task_loop_run++);
#endif
	}

	return 0;
}

/* task_loop */
#define TASK_GETCHAR	1<<0
#define TASK_BACKLIGHT	1<<1
#define TASK_LCD        1<<2
#define TASK_BUZZ		1<<3
#define TASK_SLEEP		1<<4

void inline set_task(unsigned long *taskset, int task)
{
    *taskset |= 1<<task;
}

void inline clr_task(unsigned long *taskset, int task)
{
    *taskset &= ~(1<<task);
}

int inline test_task(unsigned long taskset, int task)
{
    return taskset & (1<<task);
}

void task_init(unsigned long *taskset)
{
    *taskset = 0;
    set_task(taskset, TASK_UART);
    set_task(taskset, TASK_TIMER);
}

void task_loop(unsigned long *taskset)
{
    int task[1] = {0};

    /* uart support */
    if(test_task(*taskset, TASK_UART)) {
        int i = 0xff;
        uart_init();
        while(i--);
        uart_send_string("====== uart support ! ======\r\n");
        clr_task(taskset, TASK_UART);
    }

	/* test getchar */
	if(*task & TASK_GETCHAR) {
		char c;
		printf("test getchar------------->\n");
		c = getchar();
		printf("input char is -----------> [%c]\n", c);
	}
	/* test backlight */
	if(*task & TASK_BACKLIGHT) {
		int i;
		printf("test backlight----------->\n");
		backlight_init(BL_LEV_MIN);
		for(i = BL_LEV_MIN; i < BL_LEV_MAX; i++) {
			printf("backlight level is 0x%p\n", i);
			backlight_set_level(i);
			getchar();
		}
	}
	/* test LCD */
    if(*task & TASK_LCD) {
        int i;
        printf("test lcd------------------>\n");
        lcd_init();
        vid_toggle(1);
        for(i=0; i<800; i++) {
            printf("<><><><><><><><><><><><>\n");
            sleep(1000);
            printf("test lcd----- fb[i] = %x------------->\n", fb[i]);
        }
    }
	/* test buzz */
	if(*task & TASK_BUZZ) {
        printf("test buzz------------------>\n");
		struct timer timer;
		timer_default_cfg(&timer);
		timer.sn = TIMER1;
		timer.auto_reload = TIMER_INTERVAL;
		timer_init(&timer);
		timer_toggle(timer.sn, 1);
	}
	/* test timer sleep */
	if(*task & TASK_SLEEP) {
		int i;
		printf("test sleep-------------------->\n");
		for(i=0; i<9; i++) {
			printf("test sleep count %x-------------------->\n", i);
			timer_spin_lock(TIMER4, 1000);
		}
	}

    /* test the timer by 1Hz buzz */
    if(test_task(*taskset, TASK_TIMER)) {
        struct timer timer;
        timer_default_cfg(&timer);
        timer.sn = TIMER1;
        timer.auto_reload = TIMER_INTERVAL;
        timer_init(&timer);
        timer_toggle(timer.sn, 1);
        clr_task(taskset, TASK_TIMER);
    }
}

/* task_loop end */



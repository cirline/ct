//#define DEBUG
#define pr_fmt(fmt)	"main: "fmt

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
#include "list.h"

static LIST_HEAD_INIT(gcmds_head);

static struct shell_command * sc_malloc(void)
{
	static struct shell_command g_cmds[16];
	int i;

	for(i = 0; i < 16; i++)
		if(!g_cmds[i].cmd)
			return g_cmds + i;

	return NULL;

}

int register_shell_command(struct shell_command *sc)
{
	list_add_tail(&gcmds_head, &sc->list);

	return 0;
}

int register_shell_command_quick(char *cmd, int (*func)(void *), char *msg)
{
	struct shell_command *sc = sc_malloc();

	if(!sc) {
		pr_err("sc_malloc failed\n");
		return -1;
	}

	sc->cmd = cmd;
	sc->process = func;
	sc->help_msg = msg;

	pr_info("register command %s\n", sc->cmd);

	return register_shell_command(sc);
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

int do_help(void * p)
{
	struct shell_command *sc;
	struct list_head *l;
	int i = 0;

	for(l = gcmds_head.next; l != &gcmds_head; l = l->next, i++) {
		sc = container_of(l, struct shell_command, list);
		printf(" %s - %s\n", sc->cmd, sc->help_msg);
	}

	printf("  total %x commands\n", i);

	return 0;
}

int shell_query(void)
{
	char buffer[2048];
	int rc;
	struct shell_command *sc;
	struct list_head *l;

	getstr(buffer);

	if(! *buffer) {
		/* nothing to be done */
		rc = 0;
	} else {
		rc = -1;
		for(l = gcmds_head.next; l != &gcmds_head; l = l->next) {
			sc = container_of(l, struct shell_command, list);
			if(! strcmp(buffer, sc->cmd)) {
				/* process command */
				rc = sc->process ? sc->process(NULL) : 0;
			}
		}
		if(rc < 0)
			printf(" '%s' not found, 'help' for more information\n", buffer);
	}

	return rc;
}

int main(void)
{
	int val=0;
	unsigned char buf[2048];
	rtc_t rtc;
	int i;
	int rc;

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

#endif

	uart_init();
	for(i = 0xff; i > 0; i--) {
	}
	pr_info("====== uart support ! ======\n");

	register_shell_command_quick("help", do_help, "show this message");
	//register_shell_command_quick("exit", NULL, "exit");
	register_shell_command_quick("buzz", timer_1hz_buzz_test, "1hz buzz");

	for(rc = 0; rc <= 0; ) {
		printf("$ ");
		rc = shell_query();
		pr_debug("rc = %x\n", rc);
#if 0
		rtc_print();
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
#endif
	}
	pr_err("sys panic\n");
	while(1);

	return 0;
}

void inline clr_task(unsigned long *taskset, int task)
{
    *taskset &= ~(1<<task);
}

int inline test_task(unsigned long taskset, int task)
{
    return taskset & (1<<task);
}

int test_lcd(unsigned long *taskset)
{
    int i;
    printf("--- test lcd ---\n");
    lcd_init();
    vid_toggle(1);
    for(i=0; i<800; i++) {
        printf("<><><><><><><><><><><><>\n");
        sleep(1000);
        printf("test lcd----- fb[i] = %x------------->\n", fb[i]);
    }
    return 0;
}

int test_delay(unsigned long *taskset)
{
    int i;
    printf("--- test delay ---\n");
    for(i=0; i<9; i++) {
        printf("test sleep count %x ...\n", i);
        mdelay(1000);
        //timer_spin_lock(TIMER4, 1000);
    }
    return 0;
}

void task_loop(unsigned long *taskset)
{
    /* test timer delay */
    test_task(*taskset, TASK_DELAY) && test_delay(taskset);

	/* test backlight */
	if(test_task(*taskset, TASK_BACKLIGHT)) {
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
    test_task(*taskset, TASK_LCD) && test_lcd(taskset);
}

/* task_loop end */


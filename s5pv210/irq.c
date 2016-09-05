#include "common.h"
#include "s5p_regs.h"
#include "irq.h"
#include "uart.h"

static inline irq_class_t irq_class(unsigned int channel)
{
	return (channel >> IC_OFFSET);
}

static inline unsigned int irq_minor(unsigned int channel)
{
	return (channel & IRQMINORMASK);
}

static int irq_set_timer(unsigned int minor, irq_func_t func)
{
	pr_info("timer interrupt minor = %p\n", minor);
	if(0 <= minor && minor <=4) {
		region_write(TINT_CSTAT, 0x1, minor, 0x1);
		region_write(VICxINTENABLE(0), 0x1, 21 + minor, 0x1);
		__raw_write(VICxVECTADDRx(0, 21 + minor), (addr_t)func);
	} else if(minor == MINOR_RTCALM){
		region_write(VICxINTENABLE(0), 0x1, 28, 0x1);
		__raw_write(VICxVECTADDRx(0, 28), (addr_t)func);
	} else {
		pr_err("error irq timer minor 0x%p\n", minor);
	}

	return 0;
}

static int irq_set_ext(unsigned int minor, irq_func_t func)
{
	pr_info("external interrupt minor ... 0x%p \n", minor);
	if(0 <= minor && minor < 8) {
		region_write(GPHxCON(0), 0xf, minor<<2, 0xf);
		region_write(EXT_INT_x_CON(0), 0x7, minor<<2, 0x2);
		region_write(EXT_INT_x_MASK(0), 0x1, minor, 0x0);
		region_write(VICxINTENABLE(0), 0x1, minor, 0x1);
		__raw_write(VICxVECTADDRx(0, minor), (addr_t)func);
	} else if (16 <= minor && minor < 24){
		minor -= 16;
		region_write(GPHxCON(2), 0xf, minor<<2, 0xf);
		region_write(EXT_INT_x_CON(2), 0x7, minor<<2, 0x2);
		region_write(EXT_INT_x_MASK(2), 0x1, minor, 0x0);
		region_write(VICxINTENABLE(0), 0x1, 16, 0x1);
		__raw_write(VICxVECTADDRx(0, 16), (addr_t)func);
	} else {
		printf("error irq minor 0x%p\n", minor);
	}
	return 0;
}

static int irq_set_uart(unsigned int minor, irq_func_t func)
{
	pr_info("uart interrupt minor ... 0x%p\n", minor);
	if(0 <= minor && minor < 4) {
		uart_irq_init(func);
	} else {
		printf("error irq minor 0x%p\n", minor);
	}
}

/**
 * irq_init - set interrupt service routine
 *
 * @channel: irq channel, define in irq.h
 * @func: isr function
 */
int irq_init(unsigned int channel, irq_func_t func)
{
	pr_info("IRF(%x) address 0x%p\n", channel, func);
	switch(irq_class(channel)) {
		case ic_ext:
			irq_set_ext(irq_minor(channel), func);
			break;
		case ic_timer:
			irq_set_timer(irq_minor(channel), func);
			break;
		case ic_uart:
			irq_set_uart(irq_minor(channel), func);
			break;
		default:
			pr_err("unknown irq channel class! 0x%p\n", channel);
			break;
	}

	return 0;
}

static inline void timer_clear_pending(unsigned int minor)
{
	set2clear(TINT_CSTAT, 0x1, 5 + minor);
	region_write(TINT_CSTAT, 0x1, minor, 0x1);
}

/**
 * irq_clear_pending - clear the irq pending
 * @channel: same as irq_init() channel
 */
void irq_clear_pending(unsigned int channel)
{
	switch(irq_class(channel)) {
		case ic_timer:
			timer_clear_pending(irq_minor(channel));
			break;
	}
}

void inline eint_pending(int n, int index)
{
	set2clear(EXT_INT_x_PEND(n), 0x1, index);
}

void inline uint_pending(int n)
{
	uart_clear_int_pend(n);
}

/**
 * 1. VIC0 ~ VIC3
 * 2. write 0 to VICADDRESS when clear interrupt pending
 */
void irq_handler(void)
{
	int n;

//	fstart();
//	printf("vic0 address 0x%p\n", __raw_read(VICxADDRESS(0)));
//	printf("vic1 address 0x%p\n", __raw_read(VICxADDRESS(1)));
	if(__raw_read(VICxADDRESS(0))) {
		n = ((irq_func_t)__raw_read(VICxADDRESS(0)))();
	} else {
		n = ((irq_func_t)__raw_read(VICxADDRESS(1)))();
	}
	__raw_write(VICxADDRESS(0), 0);
	__raw_write(VICxADDRESS(1), 0);
//	fend();
}


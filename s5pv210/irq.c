#include "common.h"
#include "s5p_regs.h"
#include "irq.h"
#include "uart.h"

int irq_init(int channel, irq_func_t func)
{
	printf("IRF address 0x%p\n", func);
	switch(channel & INTCLASSMASK) {
		case int_ext:
			channel &= INTMINORMASK;
			debug("external interrupt channel ... 0x%p \n", channel);
			if(0 <= channel && channel < 8) {
				region_write(GPHxCON(0), 0xf, channel<<2, 0xf);
				region_write(EXT_INT_x_CON(0), 0x7, channel<<2, 0x2);
				region_write(EXT_INT_x_MASK(0), 0x1, channel, 0x0);
				region_write(VICxINTENABLE(0), 0x1, channel, 0x1);
				__raw_write(VICxVECTADDRx(0, channel), (addr_t)func);
			} else if (16 <= channel && channel < 24){
				channel -= 16;
				region_write(GPHxCON(2), 0xf, channel<<2, 0xf);
				region_write(EXT_INT_x_CON(2), 0x7, channel<<2, 0x2);
				region_write(EXT_INT_x_MASK(2), 0x1, channel, 0x0);
				region_write(VICxINTENABLE(0), 0x1, 16, 0x1);
				__raw_write(VICxVECTADDRx(0, 16), (addr_t)func);
			} else {
				printf("error irq channel 0x%p\n", channel);
			}
			break;
		case int_timer:
			channel &= INTMINORMASK;
			debug("timer interrupt channal ... 0x%p\n", channel);
			if(0 <= channel && channel <=4) {
				region_write(TINT_CSTAT, 0x1, channel, 0x1);
				region_write(VICxINTENABLE(0), 0x1, 21 + channel, 0x1);
				__raw_write(VICxVECTADDRx(0, 21 + channel), (addr_t)func);
			} else if(channel == MINOR_RTCALM){
				region_write(VICxINTENABLE(0), 0x1, 28, 0x1);
				__raw_write(VICxVECTADDRx(0, 28), (addr_t)func);
			} else {
				printf("error irq channel 0x%p\n", channel);
			}
			break;
		case int_uart:
			channel &= INTMINORMASK;
			debug("uart interrupt channel ... 0x%p\n", channel);
			if(0 <= channel && channel < 4) {
				uart_irq_init(func);
			} else {
				printf("error irq channel 0x%p\n", channel);
			}
			break;
		default:
			printf("unknown irq channel class! 0x%p\n", channel);
			break;
	}

	return 0;
}

void inline eint_pending(int n, int index)
{
	set2clear(EXT_INT_x_PEND(n), 0x1, index);
}

void inline tint_pending(int n)
{
	set2clear(TINT_CSTAT, 0x1, 5 + n);
	region_write(TINT_CSTAT, 0x1, n, 0x1);
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


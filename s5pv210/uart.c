/*
 * Copyright (c) 2015 Qiwei Chen
 */
//#define DEBUG
#define pr_fmt(fmt)	"uart: " fmt

#include "s5p_regs.h"
#include "irq.h"
#include "utils.h"

int uart_irq_init(irq_func_t func)
{
	// RX interrupt type
	region_write(UCONx(0), 0x1, 8, 1);
	region_write(UCONx(0), 0x1, 7, 0);
	region_write(UCONx(0), 0x3, 0, 0x1);
	__raw_write(UINTMx(0), 0xe);
	__raw_write(VICxVECTADDRx(1, 10), (addr_t)func);
	region_write(VICxINTENABLE(1), 0x1, 10, 0x1);
	fend();

	return 0;
}

void uart_clear_int_pend(int n)
{
	set2clear(UINTSPx(n), 0xf, 0);
	set2clear(UINTPx(n), 0xf, 0);
}

void uart_wait_free(void)
{
    while(! (ioread32(UTRSTATx(0)) & (1<<2)));
}

int uart_send_char(char c)
{
	uart_wait_free();
    iowrite32(c, UTXHx(0));
	return 0;
}

int uart_send_string(char *s)
{
	while(*s) {
		uart_send_char(*s);
		s++;
	}
	return 0;
}

int inline uart_rx_ready(void)
{
	return __raw_read(UTRSTATx(0)) & (1<<0);
}

unsigned char uart_get_char(void)
{
	pr_here();
	while(!uart_rx_ready());

	return __raw_read(URXHx(0)) & 0xff;
}


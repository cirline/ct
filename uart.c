/*
 * Copyright (c) 2015 Qiwei Chen
 */

#include "s5p_regs.h"
#include "irq.h"
#include "common.h"

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

int uart_init(void)
{
	__s5p_wirte(RGPIO_BASE + GPA0CON, 0x22);
	__s5p_wirte(RUART_BASE + ULCON0, 0x3);
	__s5p_wirte(RUART_BASE + UCON0, (1<<2));
	__s5p_wirte(RUART_BASE + UFCON0, 0);
	__s5p_wirte(RUART_BASE + UMCON0, 0);
//	__s5p_wirte(RCMU_BASE + CLK_SRC4, 0x7 << 16);
//	__s5p_wirte(RCMU_BASE + CLK_DIV4, 0x5 << 16);
	__s5p_wirte(RUART_BASE + UBRDIV0, 0x22);
	__s5p_wirte(RUART_BASE + UDIVSLOT0, 0xDFDD);
	return 0;
}

void uart_wait_free(void)
{
	while (!(__s5p_read(RUART_BASE + UTRSTAT0) & (1<<2)));
}

int uart_send_char(char c)
{
	uart_wait_free();
	__s5p_wirte(RUART_BASE + UTXH0, c);
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

void inline uart_rx_wait(void)
{
	while(! (__raw_read(UTRSTATx(0)) & (1<<0)));
}

unsigned char uart_get_char(void)
{
	fstart();
	uart_rx_wait();
	return __raw_read(URXHx(0)) & 0xff;
}


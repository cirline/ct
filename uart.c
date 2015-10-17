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
    region_write(GPAxCON(0), MASK_BITS_8, 0, 0x22); // pin config
    __raw_write(ULCONx(0), 3);          // line control
    __raw_write(UCONx(0), (1<<2) | 1);  // rx & tx control
    __raw_write(UFCONx(0), 0);          // fifo control
    __raw_write(UMCONx(0), 0);            // rts & cts (modem) control
//	__s5p_wirte(RCMU_BASE + CLK_SRC4, 0x7 << 16);
//	__s5p_wirte(RCMU_BASE + CLK_DIV4, 0x5 << 16);
    __raw_write(UBRDIVx(0), 0x22);        // baud rate division
    __raw_write(UDIVSLOTx(0), 0xdfdd);    // dividing slot (baud rate fine tune)
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

int inline uart_rx_ready(void)
{
	return __raw_read(UTRSTATx(0)) & (1<<0);
}

unsigned char uart_get_char(void)
{
    int ret = 0;
	fstart();
    while(!uart_rx_ready());

	return __raw_read(URXHx(0)) & 0xff;
}


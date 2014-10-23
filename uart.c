#include "s5p_regs.h"

int uart_init(void)
{
	__s5p_wirte(RGPIO_BASE + GPA0CON, 0x22);
	__s5p_wirte(RUART_BASE + ULCON0, 0x3);
	__s5p_wirte(RUART_BASE + UCON0, ((1<<2)|(1<<0)));
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


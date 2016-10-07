#ifndef __UART_H__
#define __UART_H__

#include "irq.h"

#define ULCON_WL5	0
#define ULCON_WL6	1
#define ULCON_WL7	2
#define ULCON_WL8	3
#define ULCON_SB1	0
#define ULCON_SB2	1
#define ULCON_PM_NONE	0
#define ULCON_PM_ODD	4
#define ULCON_PM_EVEN	5
#define ULCON_PM_PF1	6
#define ULCON_PM_PF0	7

struct uart_cfg {
	int uart;
	int word_length:2;
	int n_stopbit:1;
	int parity:3;

	int tx_dma_bs:1;	// tx dma burst size
	int rx_dma_bs:1;
	int clk_sel:1;
	int tx_int_type:1;
	int rx_int_type:1;
	int rx_timeout_enable:1;
	int rx_esi_enable:1;	// rx error status interrupt enable
	int tx_mode:2;
	int rx_mode:2;

	int tx_fifo_tri:3;
	int rx_fifo_tri:3;
	int tx_fifo_rst:1;
	int rx_fifo_rst:1;
	int fifo_enable:1;

	int rts_tri:3;
	int afc_enable:1;
	int mdm_int_enable:1;

	int brdiv:16;
	int divslot:16;
};

struct uart_stat {
	int txer_empty:1;
	int txbuf_empty:1;
	int rxbuf_ready:1;

	int bs:1;	// break signal
	int fe:1;	// frame error
	int pe:1;	// parity error
	int oe:1;	// overrun error

	int tx_fifo_full:1;
	int tx_fifo_cnt:8;
	int rx_fifo_err:1;
	int rx_fifo_full:1;
	int rx_fifo_cnt:8;

	int txd:8;
	int rxd:8;
};

extern int uart_init(void);
extern void uart_wait_free(void);
extern int uart_send_char(char c);
extern int uart_send_string(char *s);
extern int uart_irq_init(irq_func_t func);
extern void uart_clear_int_pend(int n);
extern unsigned char uart_get_char(void);
extern char * uart_getstr(char *buffer, int echo);
#endif

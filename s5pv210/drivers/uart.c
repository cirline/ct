/**
 * Copyright (c) 2016 chenqiwei
 */

#define pr_fmt(fmt)	"uart    - "

#include "utils.h"
#include "gpio.h"
#include "arch/s5p_regs.h"
#include "arch/s5p_uart.h"

#if 0
static int udivsolt[] = {
	UART_DIVSLOT_1n1,
	UART_DIVSLOT_1n2,
	UART_DIVSLOT_1n3,
	UART_DIVSLOT_1n4,
	UART_DIVSLOT_1n5,
	UART_DIVSLOT_1n6,
	UART_DIVSLOT_1n7,
	UART_DIVSLOT_1n8,
	UART_DIVSLOT_1n9,
	UART_DIVSLOT_1n10,
	UART_DIVSLOT_1n11,
	UART_DIVSLOT_1n12,
	UART_DIVSLOT_1n13,
	UART_DIVSLOT_1n14,
	UART_DIVSLOT_1n15
};
#endif

static struct uart_brtable uart_brt[] = {
	{B115200, 34, UART_DIVSLOT_1n13},
};

static void uart_init_gpio(int uart)
{
	switch(uart) {
		case 0:
			gpio_set_config('A', 0, 0, GPCON_CTRL);
			gpio_set_config('A', 0, 1, GPCON_CTRL);
			break;
	}
}

/**
 */
void uart_do_init(struct uart_cfg *cfg)
{
	unsigned long regval;

	/* set gpio */
	uart_init_gpio(cfg->uart);

	/* line control, set ULCON */
	regval = cfg->word_length | (cfg->n_stopbit << 2) | (cfg->parity << 3);
	__raw_write(ULCONx(cfg->uart), regval);

	/**
	 * uart control, set UCON
	 * first, set rx ...
	 */
	regval = (cfg->rx_mode) | \
		 (cfg->rx_esi_enable << 6) | \
		 (cfg->rx_timeout_enable << 7) | \
		 (cfg->rx_int_type << 8) | \
		 (cfg->rx_dma_bs << 16);
	/* second, set tx ... */
	regval |= (cfg->tx_mode << 2) | \
		  (cfg->tx_int_type << 9) | \
		  (cfg->tx_dma_bs << 20);
	/* third, set clock selection */
	regval |= cfg->clk_sel << 10;
	__raw_write(UCONx(cfg->uart), regval);

	/* fifo control, set UFCON */
	regval = cfg->fifo_enable | \
		 (cfg->rx_fifo_tri << 4) | \
		 (cfg->tx_fifo_tri << 8);
	__raw_write(UFCONx(cfg->uart), regval);

	//__raw_write(UMCONx(0), 0);            // rts & cts (modem) control

	/* set baudrate */
	__raw_write(UBRDIVx(cfg->uart), cfg->brdiv);
	__raw_write(UDIVSLOTx(cfg->uart), cfg->divslot);
}

void uart_set_line_contrl(struct uart_cfg *cfg, int wlength, int n_stopbit, int parity)
{
	cfg->word_length = wlength;
	cfg->n_stopbit = n_stopbit;
	cfg->parity = parity;
}

void uart_set_xmode(struct uart_cfg *cfg, int tx_mode, int rx_mode)
{
	cfg->rx_mode = rx_mode;
	cfg->tx_mode = tx_mode;
}

/**
 * uart_set_rx	- set receive
 * @cfg		: -
 * @xmode	: rx mode
 * @esi		: rx error status interrupt enable
 * @toe		: rx timeout enable
 * @int_type	: rx interrupt type
 * @dma_bs	: rx dma burst size
 */
void uart_set_rx(struct uart_cfg *cfg, int xmode, int esi, int toe, int int_type, int dma_bs)
{
	cfg->rx_mode = xmode;
	cfg->rx_esi_enable = esi;
	cfg->rx_timeout_enable = toe;
	cfg->rx_int_type = int_type;
	cfg->rx_dma_bs = dma_bs;
}

/**
 * uart_set_tx	- set transmit
 * @cfg		: -
 * @xmode	: tx mode
 * @intt	: tx interrupt type
 * @dma_bs	: tx dma burst size
 */
void uart_set_tx(struct uart_cfg *cfg, int xmode, int intt, int dma_bs)
{
	cfg->tx_mode = xmode;
	cfg->tx_int_type = intt;
	cfg->tx_dma_bs = dma_bs;
}

/**
 * uart_set_clock_selection	- select pclk or sclk_uart for uart baud rate
 * @selection			: pclk or sclk_uart
 *
 * div_val1 = (@selection / (bps * 16)) - 1
 */
void uart_set_clock_selection(struct uart_cfg *cfg, int selection)
{
	cfg->clk_sel = selection;
}

/**
 * uart_set_fifocon	- set fifo control
 * @en			: fifo enable
 * @rx_trigger		: rx fifo trigger level
 * @tx_trigger		: tx fifo trigger level
 *
 * while:
 *  * rx fifo data >= rx_trigger, rx interrupt occurs
 *  * tx fifo data <= tx_trigger, tx interrupt occurs
 */
void uart_set_fifocon(struct uart_cfg *cfg, int en, int rx_trigger, int tx_trigger)
{
	cfg->fifo_enable = en;
	cfg->rx_fifo_tri = rx_trigger;
	cfg->tx_fifo_tri = tx_trigger;
}

void uart_set_modemcon(void)
{
}

/**
 * uart_transmitter_is_empty - uart transmit buffer and shift register is empty
 */
static int inline uart_transmitter_is_empty(int uart)
{
	return ((__raw_read(UTRSTATx(uart)) & 0x4) != 0);
}

/**
 * uart_txbuf_is_empty - uart transmit buffer is empty
 */
static int inline uart_txbuf_is_empty(int uart)
{
	return ((__raw_read(UTRSTATx(uart)) & 0x2) != 0);
}

/**
 * uart_rxbuf_is_ready - receive buffer contain data from RXD port
 */
static int inline uart_rxbuf_is_ready(int uart)
{
	return (__raw_read(UTRSTATx(uart)) & 0x1);
}

/**
 * uart_error_status	- return uart error status
 */
static int inline uart_error_status(int uart)
{
	return __raw_read(UERSTATx(uart));
}

/**
 * uart_rx_fifocnt - uart rx fifo count
 *
 * function return -1 if rx fifo is full, or number of data when not full
 */
static int inline uart_rx_fifocnt(int uart)
{
	int regval;
	regval = __raw_read(UFSTATx(uart));
	if(regval & 0x10)
		return -1;
	else
		return regval & 0xff;
}

/**
 * uart_tx_fifocnt	- uart tx fifo count
 *
 * function return -1 if tx fifo is full, or number of when not full
 */
static int inline uart_tx_fifocnt(int uart)
{
	int regval;
	regval = __raw_read(UFSTATx(uart));
	if(regval & (1<<24))
		return -1;
	else
		return (regval>>16) & 0xff;
}

/**
 * uart_set_txbreg	- set transmit buffer register
 */
static void inline uart_set_txbreg(int uart, char c)
{
	__raw_write(UTXHx(uart), c);
}

/**
 * uart_get_rxbuf	- get receive buffer register
 */
static char inline uart_get_rxbreg(int uart)
{
	return __raw_read(URXHx(uart)) & 0xff;
}

static int uart_set_clock(struct uart_cfg *cfg, int baudrate)
{
	int i;

	for(i = 0; i < ARRAY_SIZE(uart_brt); i++) {
		if(baudrate == uart_brt[i].baudrate) {
			cfg->brdiv = uart_brt[i].brdiv;
			cfg->divslot = uart_brt[i].divslot;
			return 0;
		}
	}

	pr_err("unsupported baudrate: %x\n", baudrate);
	return -1;
}

int uart_init(void)
{
	struct uart_cfg cfg;

	memset(&cfg, 0, sizeof(struct uart_cfg));
	cfg.uart = 0;
	uart_set_line_contrl(&cfg, UART_LCON_WL8, UART_LCON_SB1, UART_LCON_PM_NONE);
	uart_set_rx(&cfg, UART_XMODE_IROP, 0, 0, UART_INTT_PULSE, UART_DBS_1b);
	uart_set_tx(&cfg, UART_XMODE_IROP, UART_INTT_PULSE, UART_DBS_1b);
	uart_set_fifocon(&cfg, 0, UART_RFTL0_32b, UART_TFTL0_0b);
	uart_set_clock(&cfg, B115200);

	uart_do_init(&cfg);
}

/**
 * uart_getstr - get a string for uart
 * @buffer: string buffer
 * @echo: is echo the char
 */
char * uart_getstr(char *buffer, int echo)
{
	int i;

	for(i = 0; (buffer[i] = uart_get_char()) != '\r'; i++) {
		if(buffer[i] == 0x7f) {
			/* backspace */
			if(i == 0)
				i--;
			else {
				i -= 2;
				if(echo)
					printf("\b \b");
			}
		} else if(echo) {
			uart_send_char(buffer[i]);
		}
	}
	buffer[i] = 0;
	if(echo)
		uart_send_string("\r\n");

	return buffer;
}


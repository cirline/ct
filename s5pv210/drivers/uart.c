/**
 * Copyright (c) 2016 chenqiwei
 */

#define pr_fmt(fmt)	"uart    - "

#include "s5p_regs.h"

/**
 */
void uart_init(struct uart_cfg *cfg)
{
	unsigned int regval;

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
		 (cfg->dma_bs << 16);
	/* second, set tx ... */
	regval |= (cfg->tx_mode << 2) | \
		  (cfg->tx_int_type << 9) | \
		  (cfg->tx_dma_bs << 20);
	/* third, set clock selection */
	regval |= cfg->clk_sel << 10;

	__raw_write(UCONx(cfg->uart), regval);

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

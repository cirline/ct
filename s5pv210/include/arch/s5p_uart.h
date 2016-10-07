#ifndef __S5P_UART__
#define __S5P_UART__

#define UART_XMODE_MASK		2
#define UART_XMODE_DISABLE	0
#define UART_XMODE_IROP		1	// interrupt request or poll mode
#define UART_XMODE_DMA		2

#define UART_ESI_MASK		1	// rx error status interrupt

#define UART_TOE_MASK		1	// rx timeout enable

#define UART_INTT_MASK		1	// RX/TX interrupt type
#define UART_INTT_PULSE		0
#define UART_INTT_LEVEL		1

#define UART_DBS_MASK		1	// dma burst size
#define UART_DBS_1b		0
#define UART_DBS_4bs		1

#define UART_CLKSEL_MASK	1	// uart baud rate clock selection
#define UART_CLKSEL_PCLK	0
#define UART_CLKSEL_SCLK_UART	1

#endif

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

#define UART_TFTL_MASK		7	// tx fifo trigger level
#define UART_TFTL0_0b		0
#define UART_TFTL0_32b		1
#define UART_TFTL0_64b		2
#define UART_TFTL0_96b		3
#define UART_TFTL0_128b		4
#define UART_TFTL0_160b		5
#define UART_TFTL0_192b		6
#define UART_TFTL0_224b		7
#define UART_TFTL1_0b		0
#define UART_TFTL1_8b		1
#define UART_TFTL1_16b		2
#define UART_TFTL1_24b		3
#define UART_TFTL1_32b		4
#define UART_TFTL1_40b		5
#define UART_TFTL1_48b		6
#define UART_TFTL1_56b		7
#define UART_TFTL2_3_0b		0
#define UART_TFTL2_3_2b		1
#define UART_TFTL2_3_4b		2
#define UART_TFTL2_3_6b		3
#define UART_TFTL2_3_8b		4
#define UART_TFTL2_3_10b	5
#define UART_TFTL2_3_12b	6
#define UART_TFTL2_3_14b	7

#define UART_RFTL_MASK		7	// rx fifo trigger level
#define UART_RFTL0_32b		0
#define UART_RFTL0_64b		1
#define UART_RFTL0_96b		2
#define UART_RFTL0_128b		3
#define UART_RFTL0_160b		4
#define UART_RFTL0_192b		5
#define UART_RFTL0_224b		6
#define UART_RFTL0_256b		7
#define UART_RFTL1_8b		0
#define UART_RFTL1_16b		1
#define UART_RFTL1_24b		2
#define UART_RFTL1_32b		3
#define UART_RFTL1_40b		4
#define UART_RFTL1_48b		5
#define UART_RFTL1_56b		6
#define UART_RFTL1_64b		7
#define UART_RFTL2_3_2b		0
#define UART_RFTL2_3_4b		1
#define UART_RFTL2_3_6b		2
#define UART_RFTL2_3_8b		3
#define UART_RFTL2_3_10b	4
#define UART_RFTL2_3_12b	5
#define UART_RFTL2_3_14b	6
#define UART_RFTL2_3_16b	7

/* uart divisor slot n'1' table */
#define UART_DIVSLOT_1n0	0x0000
#define UART_DIVSLOT_1n1	0x0080
#define UART_DIVSLOT_1n2	0x0808
#define UART_DIVSLOT_1n3	0x0888
#define UART_DIVSLOT_1n4	0x2222
#define UART_DIVSLOT_1n5	0x4924
#define UART_DIVSLOT_1n6	0x4a52
#define UART_DIVSLOT_1n7	0x54aa
#define UART_DIVSLOT_1n8	0x5555
#define UART_DIVSLOT_1n9	0xd555
#define UART_DIVSLOT_1n10	0xd5d5
#define UART_DIVSLOT_1n11	0xddd5
#define UART_DIVSLOT_1n12	0xdddd
#define UART_DIVSLOT_1n13	0xdfdd
#define UART_DIVSLOT_1n14	0xdfdf
#define UART_DIVSLOT_1n15	0xffdf

#endif

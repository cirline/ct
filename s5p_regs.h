#ifndef __S5P_REGS_H__
#define __S5P_REGS_H__

/**    **/
#define __s5p_wirte(addr, val)		(*(volatile unsigned long *)(addr)) = (val)
#define __s5p_read(addr)			(*(volatile unsigned long *)(addr))

/**  GPIO  **/
#define RGPIO_BASE		0xE0200000

#define GPA0CON			0x00000000
#define GPC0CON			0x00000060
#define GPC0DAT			0x00000064

/**  CMU  **/
#define RCMU_BASE		0xE0100000

#define CLK_SRC4		0x00000210
#define CLK_DIV4		0x00000310

/**  UART  **/
#define RUART_BASE		0xE2900000

#define ULCON0			0x00000000
#define UCON0			0x00000004
#define UFCON0			0x00000008
#define UMCON0			0x0000000C
#define UTRSTAT0		0x00000010
#define UTXH0			0x00000020
#define URXH0			0x00000024
#define UBRDIV0			0x00000028
#define UDIVSLOT0		0x0000002C

#endif

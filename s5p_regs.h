#ifndef __S5P_REGS_H__
#define __S5P_REGS_H__

/**    **/
#define __s5p_addr_type				unsigned long
#define __s5p_wirte(addr, val)		(*(volatile __s5p_addr_type *)(addr)) = (val)
#define __s5p_read(addr)			(*(volatile __s5p_addr_type *)(addr))

/**  GPIO  **/
#define RGPIO_BASE		0xE0200000

#define GPA0CON			0x00000000
#define GPC0CON			0x00000060
#define GPC0DAT			0x00000064
#define GPH0CON			0x00000C00
#define GPH0DAT			0x00000C04

#define EXT_INT_0_CON	0x00000E00
#define EXT_INT_0_MASK	0x00000F00
#define EXT_INT_0_PEND	0x00000F40

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

/**  INT  **/
#define RINT_BASE		0xF2000000
#define VIC0INTENABLE	0x00000010
#define VIC0VECTADDR0	0x00000100
#define VIC0ADDRESS		0x00000F00

#define VICx_BASE(x)	(RINT_BASE + ((x)<<20))

#define VICxINTENABLE(x)	(VICx_BASE(x) + 0x0010)
#define VICxVECTADDRx(x, y)	(VICx_BASE(x) + 0x100 + ((y)<<2))
#define VICxADDRESS(x)		(VICx_BASE(x) + 0x0F00)

#endif


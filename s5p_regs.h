#ifndef __S5P_REGS_H__
#define __S5P_REGS_H__

/**    **/
typedef unsigned long				__s5p_addr_t;
#define __s5p_wirte(addr, val)		(*(volatile __s5p_addr_t *)(addr)) = (val)
#define __s5p_read(addr)			(*(volatile __s5p_addr_t *)(addr))

/**  GPIO  **/
#define RGPIO_BASE		0xE0200000

#define GPA0CON			0x00000000
#define GPC0CON			0x00000060
#define GPC0DAT			0x00000064

#define GPH_BASE		(RGPIO_BASE + 0xC00)
#define GPHxCON(x)		(GPH_BASE + ((x)<<5) + (0<<2))
#define GPHxDAT(x)		(GPHxCON(x) + 0x4)

#define MP0_BASE		(RGPIO_BASE + 0x2E0)
#define MP0_xCON(x)		(MP0_BASE + (((x)-1)<<5))
#define MP0_xDAT(x)		(MP0_xCON(x) + 0x04)

#define EXT_INT_x_CON(x)	(RGPIO_BASE + 0xE00 + ((x)<<2))
#define EXT_INT_x_MASK(x)	(RGPIO_BASE + 0xF00 + ((x)<<2))
#define EXT_INT_x_PEND(x)	(RGPIO_BASE + 0xF40 + ((x)<<2))

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
#define VICx_BASE(x)	(RINT_BASE + ((x)<<20))

#define VICxINTENABLE(x)	(VICx_BASE(x) + 0x0010)
#define VICxVECTADDRx(x, y)	(VICx_BASE(x) + 0x100 + ((y)<<2))
#define VICxADDRESS(x)		(VICx_BASE(x) + 0x0F00)

/**  NAND FLASH  **/
#define NF_BASE			0xB0E00000
#define NFCONF			(NF_BASE + 0x0)
#define NFCONT			(NF_BASE + 0x4)
#define NFCMMD			(NF_BASE + 0x8)
#define NFADDR			(NF_BASE + 0xC)
#define NFDATA			(NF_BASE + 0x10)
#define NFSTAT			(NF_BASE + 0x28)

#endif


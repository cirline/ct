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

#define GPD_BASE		(RGPIO_BASE + 0x0A0)
#define GPDxCON(x)		(GPD_BASE + ((x)<<5))
#define GPDxDAT(x)		(GPDxCON(x) + 0x4)

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

#define ULCONx(x)		(RUART_BASE + (x<<10))
#define UCONx(x)		(ULCONx(x) + 0x4)
#define UFCONx(x)		(ULCONx(x) + 0x8)
#define UMCONx(x)		(ULCONx(x) + 0xC)
#define UTRSTATx(x)		(ULCONx(x) + 0x10)
#define UERSTATx(x)		(ULCONx(x) + 0x14)
#define UFSTATx(x)		(ULCONx(x) + 0x18)
#define UMSTATx(x)		(ULCONx(x) + 0x1C)
#define UTXHx(x)		(ULCONx(x) + 0x20)
#define URXHx(x)		(ULCONx(x) + 0x24)
#define UBRDIVx(x)		(ULCONx(x) + 0x28)
#define UDIVSLOTx(x)	(ULCONx(x) + 0x2C)
#define UINTPx(x)		(ULCONx(x) + 0x30)
#define UINTSPx(x)		(ULCONx(x) + 0x34)
#define UINTMx(x)		(ULCONx(x) + 0x38)

/**  INT  **/
#define RINT_BASE		0xF2000000
#define VICx_BASE(x)	(RINT_BASE + ((x)<<20))

#define VICxINTENABLE(x)	(VICx_BASE(x) + 0x0010)
#define VICxINTENCLEAR(x)	(VICx_BASE(x) + 0x0014)
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

/**  TIMER  **/
#define TIMER_BASE		0xE2500000
#define TCFG0			(TIMER_BASE + 0x0)
#define TCFG1			(TIMER_BASE + 0x4)
#define TCON			(TIMER_BASE + 0x8)
#define TCNTBx(x)		(TIMER_BASE + 0xC + ((x)<<4) - ((x)<<2))
#define TCMPBx(x)		(TIMER_BASE + 0x10 + ((x)<<4) - ((x)<<2))
#define TCNTOx(x)		(TIMER_BASE + 0x14 + ((x)<<4) - ((x)<<2))
#define TINT_CSTAT		(TIMER_BASE + 0x44)

/**  RTC  **/
#define RTC_BASE		0xE2800000
#define INTP			(RTC_BASE + 0x30)
#define RTCCON			(RTC_BASE + 0x40)
#define TICCNT			(RTC_BASE + 0x44)
#define RTCALM			(RTC_BASE + 0x50)
#define ALMSEC			(RTC_BASE + 0x54)
#define ALMMIN			(RTC_BASE + 0x58)
#define ALMHOUR			(RTC_BASE + 0x5C)
#define ALMDAY			(RTC_BASE + 0x60)
#define ALMMON			(RTC_BASE + 0x64)
#define ALMYEAR			(RTC_BASE + 0x68)
#define BCDSEC			(RTC_BASE + 0x70)
#define BCDMIN			(RTC_BASE + 0x74)
#define BCDHOUR			(RTC_BASE + 0x78)
#define BCDDAY			(RTC_BASE + 0x7C)
#define BCDDAYWEEK		(RTC_BASE + 0x80)
#define BCDMON			(RTC_BASE + 0x84)
#define BCDYEAR			(RTC_BASE + 0x88)
#define CURTICCNT		(RTC_BASE + 0x90)

/**  I2C  **/
#define I2C_BASE		0xE1800000
#define I2CCONx(x)		(I2C_BASE)
#define I2CSTATx(x)		(I2CCONx(x)		+ 4)
#define I2CADDx(x)		(I2CSTATx(x)	+ 4)
#define I2CDSx(x)		(I2CADDx(x)		+ 4)
#define I2CLCx(x)		(I2CDSx(x)		+ 4)

#endif


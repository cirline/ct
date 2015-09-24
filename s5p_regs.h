#ifndef __S5P_REGS_H__
#define __S5P_REGS_H__

/**    **/
typedef unsigned long				__s5p_addr_t;
#define __s5p_wirte(addr, val)		(*(volatile __s5p_addr_t *)(addr)) = (val)
#define __s5p_read(addr)			(*(volatile __s5p_addr_t *)(addr))

/** common define **/
#define OFFSET4B(x)     ((x)<<2)
#define OFFSET8B(x)     ((x)<<3)
#define GAP32B(x)        ((x)<<4)

/**  GPIO  **/
#define RGPIO_BASE		0xE0200000
#define IOCON           0
#define IODAT           4
#define IOPUD           8
#define IODRV           12
#define IOCONPDN        16
#define IOPUDPDN        20
#define GPA0CON			0x00000000
#define GPC0CON			0x00000060
#define GPC0DAT			0x00000064

#define GPD_BASE		(RGPIO_BASE + 0x0A0)
#define GPDxCON(x)		(GPD_BASE + ((x)<<5))
#define GPDxDAT(x)		(GPDxCON(x) + 0x4)

#define GPF_BASE        (RGPIO_BASE + 0x120)
#define GPFxCON(x)      (GPF_BASE + GAP32B(x))
#define GPFxDAT(x)      (GPFxCON(x) + IODAT)
#define GPFxPUD(x)      (GPFxCON(x) + IOPUD)

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

/**  DISPLAY **/
#define VID_BASE        0xF8000000
/**
 * output format, display enable/disable
 * RGB i/f control signal
 * output format control
 * image enhancement control
 */
#define VIDCONx(x)      (VID_BASE + OFFSET4B(x))
/* output timing and display size */
#define VIDTCONx(x)     (VID_BASE + OFFSET4B(x) + 0x10)
/* window feature setting */
#define WINCONx(x)      (VID_BASE + ((x)<<2) + 0x20)
/* window shadow control */
#define SHODOWCON       (VID_BASE + 0x34)
/** OSD sfr x1~3, xA~D
 * window position setting, OSD size setting
 */
#define VIDOSDxx(x, y)  (VID_BASE + ((4+x)<<4)+ OFFSET4B(y - 'A'))
/** x1~4, y0~1, z0~2
 * source image address setting
 */
#define VIDW0xADDxBx(x, y, z)   (&((vidw_add_b##z##_t *)(VID_BASE + 0xA0))->VIDW0##x##ADD##y##B##z)
struct vidw_add_b_ {
    int VIDW00ADD0B0;
    int VIDW00ADD0B1;
    int VIDW01ADD0B0;
    int VIDW01ADD0B1;
    int VIDW02ADD0B0;
    int VIDW02ADD0B1;
    int VIDW03ADD0B0;
    int VIDW03ADD0B1;
    int VIDW04ADD0B0;
    int VIDW04ADD0B1;
    int NA1, NA2;
    int VIDW00ADD1B0;
    int VIDW00ADD1B1;
    int VIDW01ADD1B0;
    int VIDW01ADD1B1;
    int VIDW02ADD1B0;
    int VIDW02ADD1B1;
    int VIDW03ADD1B0;
    int VIDW03ADD1B1;
    int VIDW04ADD1B0;
    int VIDW04ADD1B1;
};

struct vidw_add_b2 {
    char NA1[0x2000];
    long long VIDW00ADD0B2;
    long long VIDW01ADD0B2;
    long long VIDW02ADD0B2;
    long long VIDW03ADD0B2;
    long long VIDW04ADD0B2;
    long long NA2;
    long long VIDW00ADD1B2;
    long long VIDW01ADD1B2;
    long long VIDW02ADD1B2;
    long long VIDW03ADD1B2;
    long long VIDW04ADD1B2;
};

typedef struct vidw_add_b_ vidw_add_b0_t;
typedef struct vidw_add_b_ vidw_add_b1_t;
typedef struct vidw_add_b2 vidw_add_b2_t;
/* x0~4 */
#define VIDW0xADD2(x)       (VID_BASE + 0x100 + OFFSET4B(x))
/* interrupt contro/pending */
#define VIDINTCON(x)        (VID_BASE + 0x130 + OFFSET4B(x))
/* color key setting */
#define WxKEYCONx(x, y)     (VID_BASE + 0x140 + OFFSET8B(x-1) + OFFSET4B(y))
/* color key alpha value */
#define WxKEYALPHA(x)       (VID_BASE + 0x160 + OFFSET4B(x-1))
/* dithering mode */
#define DITHMODE            (VID_BASE + 0x170)
/* window color control */
#define WINxMAP(x)          (VID_BASE + 0x180 + OFFSET4B(x))
/* palette control */
#define WPALCON_H           (VID_BASE + 0x19C)
#define WPALCON_L           (VID_BASE + 0x1A0)
/* trigger control */
#define TRIGCON             (VID_BASE + 0x1A4)
/* i80 i/f control main/sub LDI */
#define I80IFCONAx(x)       (VID_BASE + 0x1B0 + OFFSET4B(x))
#define I80IFCONBx(x)       (VID_BASE + 0x1B8 + OFFSET4B(x))
/* color gain color */
#define COLORGAINCON        (VID_BASE + 0x1C0)
/* i80 i/f LDI command control */
#define LDI_CMDCONx(x)      (VID_BASE + 0x1D0 + OFFSET4B(x))
/* LCD i80 system i/f commond control */
#define SIFCCONx(x)         (VID_BASE + 0x1E0 + OFFSET4B(x))
/* hue coefficient control */
#define HUECOEFxx(x, y)     (VID_BASE + 0x1EC + OFFSET8B(x) + OFFSET4B(y))
/* hue offset control */
#define HUEOFFSET           (VID_BASE + 0x1FC)
/* window's alpha value */
#define VIDWxALPHAx(x, y)   (VID_BASE + 0x200 + OFFSET8B(x) + OFFSET4B(y))
/* bending equation control */
#define BLENDEQx(x)         (VID_BASE + 0x240 + OFFSET4B(x))
/* bending control */
#define BLENDCON            (VID_BASE + 0x260)
/* window's RTQOS control */
#define WxRTQOSCON(x)       (VID_BASE + 0x264 + OFFSET4B(x))
/* i80 i/f LDI command */
#define LDI_CMDx(x)         (VID_BASE + 0x280 + OFFSET4B(x))
/* gamma LUT data */
#define GAMMALUT_N_x(y)     (VID_BASE + 0x37C + OFFSET4B(y>>1))
/* window's buffer start/end address, buffer size */
#define SHD_VIDW0xADD0(x)   (VID_BASE + 0x40A0 + OFFSET8B(x))
#define SHD_VIDW0xADD1(x)   (VID_BASE + 0x40D0 + OFFSET8B(x))
#define SHD_VIDW0xADD2(x)   (VID_BASE + 0x4100 + OFFSET4B(x))
/** DISPLAY end **/

#endif


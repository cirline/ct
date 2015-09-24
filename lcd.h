#ifndef __LCD_H__
#define __LCD_H__

enum backlight_level {
	BL_LEV_MIN = 0,
	BL_LEV0,
	BL_LEV1,
	BL_LEV_MAX,
};

enum display_vidcon0 {
    ENVID_F     = 0,
    ENVID       = 1,
    CLKSEL_F    = 2,
    CLKDIR      = 4,
    VCLKFREE    = 5,
    CLKVAL_F    = 6,
    CLKVALUP    = 16,
    PNRMODE     = 17,
    RGSPSEL     = 18,
    VIDOUT      = 26,
};

enum vidtcon01 {
    SPW     = 0,
    FPD     = 8,
    BPD     = 16,
    PDE     = 24,
};

enum vidtcon2 {
    HOZVAL  = 0,
    LINEVAL = 11,
};

enum vidtcon3 {
    VSYNCEN     = 31,
};

enum wincon {
    ENWIN_F     = 0,
    BPPMODE_F   = 2,
    InRGB       = 13,
    BUF_MODE    = 14,
    WSWP_F      = 15,
    HAWSWP_F    = 16,
    BYTSWP_F    = 17,
    BITSWP_F    = 18,
    ENLOCAL_F   = 22,
    BUFSEL_H    = 30,
};

enum {
    C0_EN_F     = 0,
    C1_EN_F     = 1,
    C2_EN_F     = 2,
    C3_EN_F     = 3,
    C4_EN_F     = 4,
};

enum {
    W0FISEL     = 0,
    W1FISEL     = 3,
    W2FISEL     = 6,
    W3FISEL     = 9,
    W4FISEL     = 12,
    CH0FISEL    = 16,
    CH1FISEL    = 19,
    CH2FISEL    = 22,
    CH3FISEL    = 25,
    CH4FISEL    = 28,
};

enum {
    OSD_LeftTopY_F  = 0,
    OSD_LeftTopX_F  = 11,
};

enum {
    OSD_RightBotY_F = 0,
    OSD_RightBotX_F = 11,
};

enum {
    OSDSIZE     = 0,
};

#define VBASEU_F        0
#define VBASEL_F        0
#define OFFSIZE_F       13
#define PAGEWIDTH_F     0

extern char fb[];

extern int backlight_init(enum backlight_level lev);
extern int backlight_set_level(enum backlight_level lev);
extern int lcd_init(void);
extern void inline vid_toggle(int enable);

#endif

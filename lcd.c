#include <stdio.h>
#include "s5p_regs.h"
#include "common.h"
#include "lcd.h"
#include "timer.h"

/* pre Px include 4byte */
char fb[800 * 480 * 4];

int backlight_init(enum backlight_level bl_lev)
{
	struct timer timer;

	timer_default_cfg(&timer);
	timer.sn = TIMER0;
	timer.count_buffer = BL_LEV_MAX;
	timer.high_ratio = bl_lev;
    timer.auto_reload = TIMER_INTERVAL;
	timer_init(&timer);
	timer_toggle(timer.sn, 1);
	return 0;
}


int backlight_set_level(enum backlight_level lev)
{
	timer_set_period(TIMER0, 0, lev);
	timer_toggle(TIMER0, 0);
	timer_update(TIMER0);
	timer_toggle(TIMER0, 1);
	return 0;
}

void inline vid_toggle(int enable)
{
    region_write(VIDCONx(0), MASK_BITS_1, ENVID, enable & MASK_BITS_1);
}

void vid_if_init(void)
{
    int i;
    for(i = 0; i < 4; i++)
        printf("GPFxCON(%x) = %x\n", i, GPFxCON(i));
    __raw_write(GPFxCON(0), 0x22222222);
    //__raw_write(GPFxPUD(0), 0xAAAA);
    __raw_write(GPFxCON(1), 0x22222222);
    //__raw_write(GPFxPUD(1), 0xAAAA);
    __raw_write(GPFxCON(2), 0x22222222);
    //__raw_write(GPFxPUD(2), 0xAAAA);
    region_write(GPFxCON(3), MASK_BITS_16, 0, 0x2222);
    //region_write(GPFxPUD(3), MASK_BITS_8, 0, 0xAA);
}

int lcd_init(void)
{
	backlight_init(BL_LEV_MIN);
    /* set hw interface */
    vid_if_init();
    {
        printf("vidcon0: 0x%x\n", VIDCONx(0));
        region_write(VIDCONx(0), MASK_BITS_3, VIDOUT, 0);
        region_write(VIDCONx(0), MASK_BITS_1, RGSPSEL, 0);
        region_write(VIDCONx(0), MASK_BITS_8, CLKVAL_F, 5);
        region_write(VIDCONx(0), MASK_BITS_1, CLKDIR, 1);
        region_write(VIDCONx(0), MASK_BITS_1, CLKSEL_F, 0);
    } {
        printf("vidtcon0: 0x%x\n", VIDTCONx(0));
        region_write(VIDTCONx(0), MASK_BITS_8, SPW, 3);
        region_write(VIDTCONx(0), MASK_BITS_8, FPD, 21);
        region_write(VIDTCONx(0), MASK_BITS_8, BPD, 22);
    } {
        printf("vidtcon1: 0x%x\n", VIDTCONx(1));
        region_write(VIDTCONx(1), MASK_BITS_8, SPW, 7);
        region_write(VIDTCONx(1), MASK_BITS_8, FPD, 209);
        region_write(VIDTCONx(1), MASK_BITS_8, BPD, 45);
    } {
        printf("vidtcon2: 0x%x\n", VIDTCONx(2));
        region_write(VIDTCONx(2), MASK_BITS_11, HOZVAL, 799);
        region_write(VIDTCONx(2), MASK_BITS_11, LINEVAL, 479);
    } {
        printf("vidtcon3: 0x%x\n", VIDTCONx(3));
        region_write(VIDTCONx(3), MASK_BITS_1, VSYNCEN, 1);
    } {
        printf("wincon0: 0x%x\n", WINCONx(0));
        region_write(WINCONx(0), MASK_BITS_1, BUF_MODE, 0);
        region_write(WINCONx(0), MASK_BITS_1, BUFSEL_H, 0);
        region_write(WINCONx(0), MASK_BITS_1, WSWP_F, 1);
        region_write(WINCONx(0), MASK_BITS_1, InRGB, 0);
        region_write(WINCONx(0), MASK_BITS_4, BPPMODE_F, 0xB);
        region_write(WINCONx(0), MASK_BITS_1, ENWIN_F, 1);
    } {
        printf("SHODOWCON: 0x%x\n", SHODOWCON);
        region_write(SHODOWCON, MASK_BITS_1, C0_EN_F, 1);
	} {
        printf("VIDOSD0A: 0x%x\n", VIDOSDxx(0, 'A'));
        region_write(VIDOSDxx(0, 'A'), MASK_BITS_11, OSD_LeftTopX_F, 0);
        region_write(VIDOSDxx(0, 'A'), MASK_BITS_11, OSD_LeftTopY_F, 0);
    } {
        printf("VIDOSD0B: 0x%x\n", VIDOSDxx(0, 'B'));
        region_write(VIDOSDxx(0, 'B'), MASK_BITS_11, OSD_RightBotX_F, 800);
        region_write(VIDOSDxx(0, 'B'), MASK_BITS_11, OSD_RightBotY_F, 480);
    } {
        printf("VIDOSD0C: 0x%x\n", VIDOSDxx(0, 'C'));
        region_write(VIDOSDxx(0, 'C'), MASK_BITS_24, OSDSIZE, 800 * 480);
    } {
        int color = 0x0, i;
        for(i=0; i<0x177000; i++) {
            fb[i] = color++;
            color++;
        }
        printf("VIDW00ADD0B0: 0x%x\n", (int)VIDW0xADDxBx(0, 0, 0));
        printf("VIDW00ADD0B1: 0x%x\n", (int)VIDW0xADDxBx(0, 0, 1));
        printf("VIDW00ADD0B2: 0x%x\n", (int)VIDW0xADDxBx(0, 0, 2));
        printf("VIDW00ADD1B0: 0x%x\n", (int)VIDW0xADDxBx(0, 1, 0));
        printf("VIDW00ADD1B1: 0x%x\n", (int)VIDW0xADDxBx(0, 1, 1));
        printf("VIDW00ADD1B2: 0x%x\n", (int)VIDW0xADDxBx(0, 1, 2));
        printf("VIDW00ADD2: 0x%x\n", (int)VIDW0xADD2(0));
        printf("vid fb is 0x%x\n", fb);
        __raw_write(VIDW0xADDxBx(0, 0, 0), (int)fb);
        __raw_write(VIDW0xADDxBx(0, 0, 1), (int)fb);
        __raw_write(VIDW0xADDxBx(0, 0, 2), (int)fb);
        __raw_write(VIDW0xADDxBx(0, 1, 0), (int)fb + 0x177000);
        __raw_write(VIDW0xADDxBx(0, 1, 1), (int)fb + 0x177000);
        __raw_write(VIDW0xADDxBx(0, 1, 2), (int)fb + 0x177000);
        __raw_write(VIDW0xADD2(0), 800);
    }
    vid_toggle(1);
    fend();
	return 0;
}


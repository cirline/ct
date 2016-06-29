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
	timer.cmp_buffer = bl_lev;
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
    region_write(VIDCONx(0), MASK_BITS_2, ENVID_F, enable ? 3 : 0);
}

/**
 * gpio pf0 ~ pf3
 * gpio number 8 ~ 8 ~ 8 ~ 4
 * gpio pull null
 * gpio sfn = 2
 * gpio drv lv4
 * ***
 * set CMU display control register 0xe0107008 = 2
 */
void vid_if_init(void)
{
    int i;
    for(i = 0; i < 4; i++)
        printf("GPFxCON(%x) = %x\n", i, GPFxCON(i));
    __raw_write(GPFxCON(0), 0x22222222);
    __raw_write(GPFxCON(1), 0x22222222);
    __raw_write(GPFxCON(2), 0x22222222);
    region_write(GPFxCON(3), MASK_BITS_16, 0, 0x2222);
}

/**
 * 1. set timing: hsync=1, vsync=1  (vidcon1)
 * 2. set clock running: fixvclk hold -> running    (vidcon1)
 * 3. set all windows zero:
 *      - zero wincon(x) & vidosd_[ABC](x)
 *      - win(x)_shadow_protect = 0 (shadowcon)
 * 4. init color key: wxkeycon1 = wxkeycon2 = 0xffffff
 * 5. calculate the pixclock by timing data
 * 6. alloc memory buffer
 * 7. ? set r/g/b position for the window's palette
 * 8. set window video mode, timing , other setting and fb_ops
 * 9. setup 24bpp r/g/b offset & length (s3c_fb_check_var)
 * 10. ??? color map
 * 11. window shadow protect
 * 12. disable window (winconx=0)
 * 14. calc & set pixclk divider (vidcon0)
 * 15. enable lcd output (vidcon0: envid & envid_f)
 * 16. set timing: vertical ,horizontal and size (vidtcon 0 & 1 & 2)
 * 17. set framebuffer start & end address (VIDWxxADDx)
 */
int lcd_init(void)
{
    unsigned int reg;

	backlight_init(BL_LEV_MIN);
    /* set hw interface */
    vid_if_init();
    {
        reg = 0<<VIDOUT;    // RGB format
        reg |= 0<<RGBSPSEL;  // RGB parallel format
        reg |= 5<<CLKVAL_F; // VCLK = HCLK / (CLKVAL + 1)
        reg |= 1<<CLKDIR;   // VCLK divide by CLKVAL_F
        reg |= 0<<CLKSEL_F; // clock source = HCLK
        printf("VIDCON0: 0x%x = 0x%x\n", VIDCONx(0), reg);
        __raw_write(VIDCONx(0), reg);
    } {
        reg = 3<<SPW;       // vertical sync pulse width
        reg |= 21<<FPD;     // vertical front porch
        reg |= 22<<BPD;     // vertical back porch
        printf("VIDTCON0: 0x%x = 0x%x\n", VIDTCONx(0), reg);
        __raw_write(VIDTCONx(0), reg);
    } {
        reg = 7<<SPW;       // horizontal SPW
        reg |= 209<<FPD;    // horizontal FPD
        reg |= 45<<BPD;     // horizontal BPD
        printf("VIDTCON1: 0x%x = 0x%x\n", VIDTCONx(1), reg);
        __raw_write(VIDTCONx(1), reg);
    } {
        reg = 799<<HOZVAL;      // horizontal size
        reg |= 479<<LINEVAL;    // vertical size
        printf("VIDTCON2: 0x%x = 0x%x\n", VIDTCONx(2), reg);
        __raw_write(VIDTCONx(2), reg);

    } {
        printf("VIDTCON3: 0x%x\n", VIDTCONx(3));
        region_write(VIDTCONx(3), MASK_BITS_1, VSYNCEN, 1);
    } {
#if 0
        region_write(WINCONx(0), MASK_BITS_1, BUF_MODE, 0);
        region_write(WINCONx(0), MASK_BITS_1, BUFSEL_H, 0);
        region_write(WINCONx(0), MASK_BITS_1, WSWP_F, 1);
        region_write(WINCONx(0), MASK_BITS_1, InRGB, 0);
        region_write(WINCONx(0), MASK_BITS_4, BPPMODE_F, 0xB);
        region_write(WINCONx(0), MASK_BITS_1, ENWIN_F, 1);
#endif
        reg = 1<<ENLOCAL_F;     // data access method is local path
        reg |= 0<<BITSWP_F;     // bit swap disable
        reg |= 0<<BYTSWP_F;     // byte swap disable
        reg |= 0<<HAWSWP_F;     // half-word swap disable
        reg |= 0<<WSWP_F;       // word swap disable
        reg |= 0xb<<BPPMODE_F;  // bpp set RGB:888 mode
        reg |= 1<<ENWIN_F;      // enable window
        printf("WINCON0: 0x%x = %x\n", WINCONx(0), reg);
        __raw_write(WINCONx(0), reg);

    } {
        reg = 1<<C0_ENLOCAL_F;  // enable channel 0 local path
        reg |= 1<<C0_EN_F;      // enable channel 0
        printf("SHODOWCON: 0x%x = 0x%x\n", SHODOWCON, reg);
        __raw_write(SHODOWCON, reg);
	} {
        reg = 0<<OSD_LeftTopX_F;
        reg |= 0<<OSD_LeftTopY_F;
        printf("VIDOSD0A: 0x%x = %x\n", VIDOSDxx(0, 'A'), reg);
        __raw_write(VIDOSDxx(0, 'A'), reg);
    } {
        reg = 800<<OSD_RightBotX_F;
        reg |= 480<<OSD_RightBotY_F;
        printf("VIDOSD0B: 0x%x = %x\n", VIDOSDxx(0, 'B'), reg);
        __raw_write(VIDOSDxx(0, 'B'), reg);
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


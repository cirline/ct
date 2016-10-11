#ifndef __DISPLAY_H__
#define __DISPLAY_H__

struct hv_config {
	int hozval;	// thd	horizontal display area
	int linehoz;	// th	horizontal one line
	int hspw;	// thpw	hsync pulse width
	int hbpd;	// thb	hsync blanking
	int hfpd;	// thfp	hsync front porch

	int lineval;	// tvd	vertical display area
	int frameline;	// tv	vsync period time
	int vspw;	// tvpw	vsync pulse width
	int vbpd;	// tvb	vsync blanking
	int vfpd;	// tvfp	vsync front porch

	int dclk;	// fclk
};

#define HVS_LOW		0
#define HVS_HIGH	1
#define HVS_BOTH	2
struct hv_signal {
	int vs, hs, dclk, de;
	unsigned char r, g, b;
};

#define D_BACKLIGHT_MAX		255

extern int display_init_backlight(int bl);
extern int display_set_backlight_level(int bl);
#endif

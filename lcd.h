#ifndef __LCD_H__
#define __LCD_H__

enum backlight_level {
	BL_LEV_MIN = 0,
	BL_LEV0,
	BL_LEV1,
	BL_LEV_MAX,
};

extern int backlight_init(enum backlight_level lev);
extern int backlight_set_level(enum backlight_level lev);
extern int lcd_init(void);

#endif

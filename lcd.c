#include <stdio.h>
#include "s5p_regs.h"
#include "common.h"
#include "lcd.h"
#include "timer.h"

int backlight_init(enum backlight_level bl_lev)
{
	timer_init(TIMER0, BL_LEV_MAX, bl_lev);
	timer_toggle(TIMER0, 1);
	return 0;
}


int backlight_set_level(enum backlight_level lev)
{
	timer_set_period(TIMER0, NULL, lev);
	timer_toggle(TIMER0, 0);
	timer_update(TIMER0);
	timer_toggle(TIMER0, 1);
	return 0;
}

int lcd_init(void)
{
	backlight_init(BL_LEV_MIN);
	return 0;
}



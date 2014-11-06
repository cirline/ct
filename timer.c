#include "common.h"
#include "s5p_regs.h"

void inline timer_update(void)
{
	region_write(TCON, 0x1, 9, 1);
}

void timer_toggle(int x)
{
	region_write(TCON, 0x1, 9, 0);
	region_write(TCON, 0x1, 8, (x & 1));
}

int timer_init(void)
{
	region_write(GPDxCON(0), 0xf, 4, 0x2);

	region_write(TCFG0, 0xff, 0, 0xff);
	region_write(TCFG1, 0xf, 4, 0x4);
//	region_write(TCON, 0xf, 8, ((1<<3)|(1<<2)));
	region_write(TCON, 0xf, 8, (1<<2));
	__raw_write(TCNTBx(1), 16110);
	__raw_write(TCMPBx(1), 8550);
	timer_update();

	return 0;
}


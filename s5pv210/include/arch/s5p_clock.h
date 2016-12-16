#ifndef __S5P_CLOCK_H__
#define __S5P_CLOCK_H__

#include "arch/s5p_regs.h"

#define PLL_LOCKED_OFFSET	29
#define PLL_MDIV_OFFSET		16
#define PLL_PDIV_OFFSET		8
#define PLL_SDIV_OFFSET		0

#define APLL_VALUE(en, m, p, s)	(((en) << 31) | \
		((m) << PLL_MDIV_OFFSET) | ((p) << PLL_PDIV_OFFSET) | ((s) << PLL_SDIV_OFFSET))

#define MPLL_VALUE(en, m, p, s)	APLL_VALUE(en, m, p, s)
#define EPLL_VALUE(en, m, p, s)	APLL_VALUE(en, m, p, s)
#define VPLL_VALUE(en, m, p, s)	APLL_VALUE(en, m, p, s)

#define FOUTAPLL	(1)
#define FOUTMPLL	(1<<4)
#define FOUTEPLL	(1<<8)
#define FOUTVPLL	(1<<12)

#endif

/**
 * XXTI
 * |
 * |`-> APLL -> SCLK_APLL ->-.
 * |`-> MPLL -> SCLK_MPLL ->-.|
 * |     _____________________/
 * |    /
 * |    |`--- MSYS Domain
 * |    | `-> MOUT_MSYS ->- ARMCLK ->- HCLK_MSYS ->- PCLK_MSYS
 * |    |
 * |    |`--- DSYS Domain
 * |    | `-> MOUT_DSYS ->- HCLK_DSYS ->- PCLK_DSYS
 * |    |
 * |     `--- PSYS Domain
 * |      `-> MOUT_PSYS ->- HCLK_PSYS ->- PCLK_PSYS
 * |
 * |`-> VPLL -> SCLK_VPLL
 *  `-> EPLL -> SCLK_EPLL
 *
 * MSYS Domain: A8 processor, dram(DMC), 3D, IRAM/IROM, INTC, ...
 * DSYS Domain: display related, FIMC, JPEG, mutilmedia IPs, ...
 * PSYS Domain: security, I/O peripherals, low power audio play(LPAP), ...
 *
 * typical:
 * ARMCLK	= 1000MHz
 * HCLK_MSYS	=  200MHz
 * PCLK_MSYS	=  100MHz
 * HCLK_DSYS	=  166MHz
 * PCLK_DSYS	=   83MHz
 * HCLK_PSYS	=  133MHz
 * PCLK_PSYS	=   66MHz
 *
 * APLL		= 1000MHz
 * MPLL		=  667MHz
 * EPLL		=   96MHz
 * VPLL		=   54MHz
 * ARMCLK	= MOUT_MSYS / (APLL_RATION + 1)		0
 * SCLKA2M	= SCLK_APLL / (A2M_RATIO + 1)		4
 * HCLK_MSYS	= ARMCLK    / (HCLK_MSYS_RATIO + 1)	4
 * PCLK_MSYS	= HCLK_MSYS / (PCLK_MSYS_RATIO + 1)	1
 * HCLK_DSYS	= MOUT_DSYS / (HCLK_DSYS_RATIO + 1)	3
 * PCLK_DSYS	= HCLK_DSYS / (PCLK_DSYS_RATIO + 1)	1
 * HCLK_PSYS	= MOUT_PSYS / (HCLK_PSYS_RATIO + 1)	4
 * PCLK_PSYS	= HCLK_PSYS / (PCLK_PSYS_RATIO + 1)	1
 */

#include "arch/s5p_clock.h"
#include "utils.h"

static int inline clock_apll_is_locked(void)
{
	return !!(__raw_read(APLL_CON(0)) & (1 << PLL_LOCKED_OFFSET));
}

static int inline clock_mpll_is_locked(void)
{
	return !!(__raw_read(MPLL_CON) & (1 << PLL_LOCKED_OFFSET));
}

static int inline clock_epll_is_locked(void)
{
	return !!(__raw_read(EPLL_CON(0)) & (1 << PLL_LOCKED_OFFSET));
}

static int inline clock_vpll_is_locked(void)
{
	return !!(__raw_read(VPLL_CON) & (1 << PLL_LOCKED_OFFSET));
}


void clock_startup(void)
{
	/* reset clock source mux to remove PLL and divider */
	__raw_write(CLK_SRC(0), 0);
	
	/* set PLL */
	__raw_write(APLL_CON(0), APLL_VALUE(1, 125, 3, 1));
	__raw_write(MPLL_CON,	 MPLL_VALUE(1, 667, 12, 1));
	__raw_write(EPLL_CON(0), EPLL_VALUE(1, 48, 3, 2));
	__raw_write(VPLL_CON,	 VPLL_VALUE(1, 108, 6, 3));

	/* wait PLL to locked */
	while(clock_apll_is_locked() &&
			clock_mpll_is_locked() &&
			clock_epll_is_locked() &&
			clock_vpll_is_locked())
		;

	/* select clock source mux, insert PLL to clock path */
	__raw_write(CLK_SRC(0), FOUTAPLL | FOUTMPLL | FOUTEPLL | FOUTVPLL);

	/* set divider */
}

void clock_reset(void)
{
	//__raw_write(CLK_SRC(0), 0);
	//__raw_write(CLK_DIV(0), 0);
}


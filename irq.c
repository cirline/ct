#include "common.h"
#include "s5p_regs.h"
#include "irq.h"

void key0_func(void)
{
	static int n = 0;
	cprint("key0 func --- 0x%p\r\n", n++);

	__s5p_wirte(VICxADDRESS(0), 0);
	__s5p_wirte(EXT_INT_x_PEND(0), 0x1);
}

int irq_init(int channel, irq_func_t func)
{
	__s5p_wirte(GPHxCON(0), (0xf<<0));
	__s5p_wirte(EXT_INT_x_CON(0), 0x2);
	__s5p_wirte(EXT_INT_x_MASK(0), 0x0);
	__s5p_wirte(VICxINTENABLE(0), 0x1);
	__s5p_wirte(VICxVECTADDRx(0, 0), (__s5p_addr_type)key0_func);
	/* set gpio function */

	/* set gpio toggle */

	/* set intrrupt context */

	return 0;
}

void irq_handler(void)
{
	cprint("irq_handler request ... \r\n");
	((void(*)(void))__s5p_read(VICxADDRESS(0)))();
}


#include "common.h"
#include "s5p_regs.h"


void key0_func(void)
{
	static int n = 0;
	cprint("key0 func --- 0x%p\r\n", n++);

	__s5p_wirte(RINT_BASE + VIC0ADDRESS, 0);
	__s5p_wirte(RGPIO_BASE + EXT_INT_0_PEND, 0x1);
}

int irq_init(void)
{
	__s5p_wirte(RGPIO_BASE + GPH0CON, (0xf<<0));
	__s5p_wirte(RGPIO_BASE + EXT_INT_0_CON, 0x2);
	__s5p_wirte(RGPIO_BASE + EXT_INT_0_MASK, 0x0);
	__s5p_wirte(VICxINTENABLE(0), 0x1);
	__s5p_wirte(VICxVECTADDRx(0, 0), (__s5p_addr_type)key0_func);

	return 0;
}

void irq_handler(void)
{
	cprint("irq_handler request ... \r\n");
	((void(*)(void))__s5p_read(RINT_BASE + VIC0ADDRESS))();
}


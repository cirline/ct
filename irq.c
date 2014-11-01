#include "common.h"
#include "s5p_regs.h"
#include "irq.h"

int irq_init(int channel, irq_func_t func)
{
	switch(channel & INTCLASSMASK) {
		case int_ext:
			channel &= (~INTCLASSMASK);
			debug("external interrupt channel ... %p \n", channel);
			if(channel < 8) {
				region_write(GPHxCON(0), 0xf, channel<<2, 0xf);
				region_write(EXT_INT_x_CON(0), 0x7, channel<<2, 0x2);
				region_write(EXT_INT_x_MASK(0), 0x1, channel, 0x0);
				region_write(VICxINTENABLE(0), 0x1, channel, 0x1);
				__raw_write(VICxVECTADDRx(0, channel), (addr_t)func);
			} else {
				printf("unknown irq channel!\n");
			}
		default:
			printf("unknown irq channel!\n");
	}

	return 0;
}

void inline eint_pending(int n, int index)
{
	set2clear(EXT_INT_x_PEND(n), 0x1, index);
}

void irq_handler(void)
{
	int n;

	printf("irq_handler request ... \r\n");
	n = ((irq_func_t)__s5p_read(VICxADDRESS(0)))();
	__s5p_wirte(VICxADDRESS(0), 0);
}


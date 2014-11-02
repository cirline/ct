#include "common.h"
#include "s5p_regs.h"
#include "irq.h"

int irq_init(int channel, irq_func_t func)
{
	switch(channel & INTCLASSMASK) {
		case int_ext:
			channel &= INTMINORMASK;
			debug("external interrupt channel ... 0x%p \n", channel);
			if(0 <= channel && channel < 8) {
				region_write(GPHxCON(0), 0xf, channel<<2, 0xf);
				region_write(EXT_INT_x_CON(0), 0x7, channel<<2, 0x2);
				region_write(EXT_INT_x_MASK(0), 0x1, channel, 0x0);
				region_write(VICxINTENABLE(0), 0x1, channel, 0x1);
				__raw_write(VICxVECTADDRx(0, channel), (addr_t)func);
			} else if (16 <= channel && channel < 24){
				channel -= 16;
				region_write(GPHxCON(2), 0xf, channel<<2, 0xf);
				region_write(EXT_INT_x_CON(2), 0x7, channel<<2, 0x2);
				region_write(EXT_INT_x_MASK(2), 0x1, channel, 0x0);
				region_write(VICxINTENABLE(0), 0x1, 16, 0x1);
				__raw_write(VICxVECTADDRx(0, 16), (addr_t)func);
			} else {
				printf("error irq channel 0x%p\n", channel);
			}
			break;
		default:
			printf("unknown irq channel class! 0x%p\n", channel);
			break;
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


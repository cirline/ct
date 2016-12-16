#include "common.h"
#include "arch/s5p_regs.h"

void gpio_set_value(char group, int index, int pin, int value)
{
	unsigned long addrcon;
	unsigned long addrdat;

	switch(group) {
		case 'c':
		case 'C':
			addrcon = GPCxCON(index);
			addrdat = GPCxDAT(index);
			break;
	}

	/* set pin is gpio */
	region_write(addrcon, MASK_BITS_4, pin << 2, 1);
	/* set value */
	region_write(addrdat, MASK_BITS_1, pin, !!value);
}

void gpio_set_config(char group, int index, int pin, int value)
{
	unsigned long addrcon;

	switch(group) {
		case 'a':
		case 'A':
			addrcon = GPAxCON(index);
			break;
	}

	region_write(addrcon, MASK_BITS_4, GAP4B(pin), value);
}


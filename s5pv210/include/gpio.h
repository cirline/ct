#ifndef __GPIO_H__
#define __GPIO_H__

#include "arch/s5p_gpio.h"

extern void gpio_set_value(char group, int index, int pin, int value);
extern void gpio_set_config(char group, int index, int pin, int value);

#endif

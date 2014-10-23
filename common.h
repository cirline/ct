#ifndef __COMMON_H__
#define __COMMON_H__

#include "uart.h"

#define low_putchar(ch)		uart_send_char(ch)

extern char *i2hs(int n, char *ds);
extern int cprint(char *s, ...);
#endif

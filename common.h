#ifndef __COMMON_H__
#define __COMMON_H__

#define DEBUG

#include "uart.h"
#include "s5p_regs.h"

#define low_putchar(ch)		uart_send_char(ch)

#define printf(format, arg...)		__s5p_printf(format, ##arg)
#define getchar()					uart_get_char()
#define __raw_write(addr, val)		__s5p_wirte(addr, val)
#define __raw_read(addr)			__s5p_read(addr)
typedef __s5p_addr_t				addr_t;

#ifdef DEBUG
#define debug(format, arg...)		printf(format, ##arg)
#define fstart()					printf("%s start ... \n", __func__)
#define fend()						printf("%s end ... \n", __func__)
#else
#define debug(format, arg...)
#define fstart()
#define fend()
#endif

extern void inline region_write(addr_t addr, int mask, int index, int val);
extern void inline set2clear(addr_t addr, int mask, int index);
extern char *i2hs(int n, char *ds);
extern int __s5p_printf(char *s, ...);
#endif

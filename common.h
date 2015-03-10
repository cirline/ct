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
#define sleep(time)					__s5p_sleep(time)
typedef __s5p_addr_t				addr_t;

#ifdef DEBUG
#define debug(format, arg...)		printf(format, ##arg)
#define fstart()					printf("function [%s] , line [%x] start ... \n", __func__, __LINE__)
#define fend()						printf("function [%s] , line [%x] end ... \n", __func__, __LINE__)
#else
#define debug(format, arg...)
#define fstart()
#define fend()
#endif

enum mask_defined {
	MASK_BITS_1 = 0x1,
	MASK_BITS_2 = 0x3,
	MASK_BITS_3 = 0x7,
	MASK_BITS_4 = 0xf,
	MASK_BITS_8 = 0xff,
	MASK_BITS_16 = 0xffff,
};

extern void inline region_write(addr_t addr, int mask, int index, int val);
extern int inline region_read(addr_t addr, int mask, int index);
extern void inline set2clear(addr_t addr, int mask, int index);
extern char *i2hs(int n, char *ds);
extern int __s5p_printf(char *s, ...);
#endif

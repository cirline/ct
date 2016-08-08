#ifndef __COMMON_H__
#define __COMMON_H__

#include "uart.h"
#include "s5p_regs.h"
#include "list.h"

#undef NULL
#define NULL	(0)

#define low_putchar(ch)		uart_send_char(ch)

#define printf(fmt, ...)	__s5p_printf(fmt, ##__VA_ARGS__)
#define scanf(fmt, ...)		__s5p_scanf(fmt, ##__VA_ARGS__)
#define getchar()					uart_get_char()
#define getstr(buffer)		uart_getstr(buffer, 1)

#define __raw_write(addr, val)		__s5p_write(addr, val)
#define __raw_read(addr)			__s5p_read(addr)
#define sleep(time)					__s5p_sleep(time)
typedef __s5p_addr_t				addr_t;

/**
 * pr series
 */
#ifndef pr_fmt
#define pr_fmt(fmt)		""fmt
#endif

#ifdef DEBUG
#define pr_debug(fmt, ...)	printf(pr_fmt(fmt), ##__VA_ARGS__)
#else
#define pr_debug(fmt, ...)
#endif
#define pr_info(fmt, ...)	printf(pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn(fmt, ...)	printf(pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...)	printf(pr_fmt(fmt), ##__VA_ARGS__)

#define pr_here(fmt)		pr_debug("%s: [0x%x] " fmt "\n", __func__,__LINE__)


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
    MASK_BITS_11 = 0x7FF,
	MASK_BITS_16 = 0xffff,
	MASK_BITS_24 = 0xffffff,
};

struct shell_command {
	char *cmd;
	int (*process)(void *p);
	char *help_msg;
	struct list_head list;
};

extern void inline mdelay(int ms);
extern unsigned int ioread32(const volatile addr_t addr);
extern void iowrite32(unsigned int w, const volatile addr_t addr);
extern void inline region_write(addr_t addr, int mask, int index, int val);
extern int inline region_read(addr_t addr, int mask, int index);
extern void inline set2clear(addr_t addr, int mask, int index);
extern char *i2hs(unsigned int n, char *ds);
extern int __s5p_printf(char *s, ...);
extern int inline __s5p_sleep(int ms);

extern int strcmp(const char *a, const char *b);
extern int register_shell_command(struct shell_command *sc);

#endif


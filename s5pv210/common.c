#define DEBUG
#define pr_fmt(fmt)	"common " fmt

#include <stdarg.h>
#include "common.h"
#include "timer.h"

void inline mdelay(int ms)
{
    __s5p_sleep(ms);
}

unsigned int ioread32(const volatile addr_t addr)
{
    return __s5p_read(addr);
}

void iowrite32(unsigned int w, const volatile addr_t addr)
{
    __s5p_write(addr, w);
}

void inline region_write(addr_t addr, int mask, int index, int val)
{
	int reg;

	reg = (~(mask << index)) & __raw_read(addr);
	__raw_write(addr, (reg | ((val & mask) << index)));
}

int inline region_read(addr_t addr, int mask, int index)
{
	return (__raw_read(addr) >> index) & mask;
}

void inline set2clear(addr_t addr, int mask, int index)
{
	__raw_write(addr, (mask << index));
}

/*    string    */
int strcmp(const char *a, const char *b)
{
	while(*a && (*a == *b)) {
		pr_debug("a = %c, b = %c\n", *a, *b);
		a++;
		b++;
	}

	pr_debug("a = %c, b = %c\n", *a, *b);
	return *a || *b;
}

char *i2hs(unsigned int n, char *ds)
{
	char *tb = "0123456789ABCDEF";
	char ts[32];
	char *tsp = ts, *dsp = ds;

	*tsp = 0;
	while(n > 0) {
		*++tsp = tb[n & 0xf];
		n >>= 4;
	}
	if(!*tsp)
		*++tsp = '0';
	while((*dsp++ = *tsp--));
	return ds;
}

/**
 * fmt
 * %s: string
 */
int __s5p_scanf(char *fmt, ...)
{
	return 0;
}

/**
 * valid variable:
 * x: hexadecimal number
 * p: hexadecimal number
 * s: string
 * c: character
 */
int __s5p_printf(char *s, ...)
{
	va_list vl;
	int flag = 0;
	char st[32];

	va_start(vl, s);
	for(; *s; s++) {
		if(flag) {
			flag = 0;
			switch(*s) {
				case '%':
					low_putchar(*s);
					break;
				case 'p':
				case 'x':
					printf(i2hs(va_arg(vl, unsigned int), st));
					break;
				case 's':
					printf(va_arg(vl, char *));
					break;
				case 'c':
					low_putchar(va_arg(vl, int));
					break;
				default:
					low_putchar('?');
			}
		} else if(*s == '%') {
			flag = 1;
		} else if(*s == '\n'){
			low_putchar('\r');
			low_putchar('\n');
		} else {
			low_putchar(*s);
		}
	}

	return 0;
}

int inline __s5p_sleep(int ms)
{
	return timer_spin_lock(TIMER4, ms);
}


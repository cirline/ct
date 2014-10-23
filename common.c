#include <stdarg.h>
#include "common.h"

char *i2hs(int n, char *ds)
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

int cprint(char *s, ...)
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
					cprint(i2hs(va_arg(vl, int), st));
					break;
				case 's':
					cprint(va_arg(vl, char *));
					break;
				default:
					low_putchar('?');
			}
		} else if(*s == '%') {
			flag = 1;
		} else {
			low_putchar(*s);
		}
	}

	return 0;
}

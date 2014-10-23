#include <stdio.h>
#include <stdarg.h>

char *myitoa(int n, char *s, int base)
{
	int nn;
	char *tb = "0123456789ABCDEF";
	char st[32];
	char *sp = s, *stp = st;

	*stp = 0;
	while(n > 0) {
		nn = n % base;
		n /= base;
		stp++;
		*stp = tb[nn];
	}
	while(*sp++ = *stp--);
	return s;
}

int myprintf(char *s, ...)
{
	char *p;
	int flag = 0;
	va_list vl;
	int vv;
	char st[32], *stp;

	va_start(vl, s);
	for(p = s; *p; p++) {
		if(flag) {
			flag = 0;
			switch(*p) {
				case '%':
					putchar(*p);
					break;
				case 'd':
					vv = va_arg(vl, int);
					myprintf(myitoa(vv, st, 10));
					break;
				case 'p':
					vv = va_arg(vl, int);
					myprintf(myitoa(vv, st, 16));
					break;
				default:
					putchar('?');
					break;
			}
		} else if(*p == '%') {
			flag = 1;
		} else {
			putchar(*p);
		}
	}
	va_end(vl);

	return 0;
}

int main(void)
{
	myprintf("hello world.\n");
	myprintf("[%%]\n");
	myprintf("[%d]\n", 1234567890);
	myprintf("[%p]\n", 1234567890);
	
	return 0;
}

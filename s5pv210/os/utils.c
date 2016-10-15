#include "../common.c"

#undef pr_fmt(fmt)
#define pr_fmt(fmt)	"[utils] " fmt

/**
 * str2hl	- string to hex long
 */
long str2hl(const char *ss)
{
	int x;
	long number;

	for(number = 0; *ss; ss++) {
		if(*ss >= 0x30 && *ss <= 0x39)
			x = *ss - 0x30;
		else if(*ss >= 0x41 && *ss <= 0x46)
			x = *ss - 0x41 + 10;
		else if(*ss >= 0x61 && *ss <= 0x66)
			x = *ss - 0x61 + 10;
		else
			return -1;

		number = (number << 4) | x;
	}

	return number;
}


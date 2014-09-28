#include <stdio.h>

#include <config/autoconf.h>
#include <mach/gpio.h>

int main(void)
{
	int i;

	printf("hello world.\n");

	for(i=0; i<8; i++)
		printf("GPA0-[%d] addr is %p\n", i, S5PV210_GPA0(i));
	for(i=0; i<5; i++)
		printf("GPA1-[%d] addr is %p\n", i, S5PV210_GPA1(i));

	return 0;
}



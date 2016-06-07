#include <stdio.h>

int get_gcd(int a, int b)
{
	while(b > 0) {
		int n = a % b;
		a = b;
		b = n;
	}
	return a;
}

int main(void)
{
	printf("36, 9 = %d\n", get_gcd(36, 9));
	return 0;
}


#include <stdio.h>
#include <stdlib.h>

int get_gcd(int a, int b)
{
	while(b > 0) {
		int n = a % b;
		a = b;
		b = n;
	}
	return a;
}

int main(int argc, char *argv[])
{
	int a, b;

	if(argc < 3) {
		printf("less argument\n");
		return -1;
	}

	a = atoi(argv[1]);
	b = atoi(argv[2]);

	printf("%d, %d = %d\n", a, b, get_gcd(a, b));
	return 0;
}


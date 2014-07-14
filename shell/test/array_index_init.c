#include <stdio.h>

int main(void)
{
	int i;
	int arr[] = {
		[0] = 5,
		[3] = 9,
	};

	for(i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
		printf("arr[%d] = %d\n", i, arr[i]);

	return 0;
}

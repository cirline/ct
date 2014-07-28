#include <stdio.h>
#include <stdlib.h>

typedef struct __so_test {
	int i;
	void (*echo_func)(struct __so_test *);
} test_t;

void _init(void);



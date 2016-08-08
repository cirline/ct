#include <stdio.h>
#include <stdlib.h>

typedef struct __so_test {
	int i;
	void (*echo_func)(struct __so_test *);
} test_t;

void __main_register(test_t *p);

static void __so_printf(test_t *p) {
	printf("i = %d\n", p->i);
}

static test_t config = {
	.i = 0,
	.echo_func = __so_printf,
};

void _init(void) {
	printf("so _init\n");
	__main_register(&config);
}


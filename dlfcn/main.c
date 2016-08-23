#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef struct __so_test {
	int i;
	void (*echo_func)(struct __so_test *);
} test_t;

void __main_register(test_t *p)
{
	p->i = 5;
	p->echo_func(p);
}

int main(void)
{
	void *handle = NULL;
	char *path = "./dlfcn.so";

	if((handle = dlopen(path, RTLD_NOW)) == NULL) {
		printf("dlopen - %s\n", dlerror());
		return -1;
	}
	return 0;
}


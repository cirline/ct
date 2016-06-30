#include <stdio.h>
#include <time.h>

const struct timespec delay = {
	.tv_sec = 0,
	.tv_nsec = 10000000,
};

int main(void)
{
	struct timespec spec;
	int rc;
	int i = 0;

	while(1) {
		rc = nanosleep(&delay, &spec);
		printf("loop = %d, i = %d\n", rc, i++);
	}
	return 0;
}

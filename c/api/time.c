/**
 * here is time relative function
 *
 * struct timeval {...}
 * struct timespec {...}
 *
 * gettimeofday()
 * nanosleep()
 */

#include <stdio.h>

#include <sys/time.h>

const struct timespec delay_1s = {
	.tv_sec = 1,
	.tv_nsec = 0,
};

const struct timespec delay_10ms = {
	.tv_sec = 0,
	.tv_nsec = 10000000,
};

int main(void)
{
	struct timeval start_time, end_time, gap_time;
	struct timespec spec;

	gettimeofday(&start_time, NULL);
	nanosleep(&delay_10ms, &spec);
	gettimeofday(&end_time, NULL);
	timersub(&end_time, &start_time, &gap_time);
	printf("10ms, %ld.%06ld\n", gap_time.tv_sec, gap_time.tv_usec);

	return 0;
}

#include <stdio.h>
#include <linux/unistd.h>
#include <linux/fcntl.h>

int main(void)
{

	struct {
		int a, b;
		char c, d;
		int e;
	} s;

	int fd;

	s.a = 0x12345678;
	s.b = 0x87654321;

	s.c = 'a';
	s.d = 'd';

	s.e = 0x13578642;

	fd = open("data", O_RDWR);
	if(fd < 0) {
		printf("file error\n");
		return -1;
	}

	write(fd, &s, sizeof(s));





	return 0;
}

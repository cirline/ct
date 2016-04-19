#include <stdio.h>


int test_locker1(void)
{
	return ! access("1.locker", F_OK);
}

void edit_locker1(int ready)
{
	if(ready) {
		int fd = creat("1.locker", 0666);
		close(fd);
	} else
		remove("1.locker");
}

int test_locker2(void)
{
	return ! access("2.locker", F_OK);
}

void edit_locker2(int ready)
{
	if(ready) {
		int fd = creat("2.locker", 0666);
		close(fd);
	} else
		remove("2.locker");
}

void print_locker(void)
{
	if(test_locker1())
		printf("locker1 found!\n");
	else
		printf("locker1 no found!\n");
	if(test_locker2())
		printf("locker2 found!\n");
	else
		printf("locker2 no found!\n");
}

int main(void)
{
	while(1) {
		edit_locker1(1);
		print_locker();
	}
}


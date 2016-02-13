#include <stdio.h>

void inline settask(unsigned int *set, int task)
{
    *set |= 1<<task;
}

void inline clrtask(unsigned int *set, int task)
{
    *set &= ~(1<<task);
}

int testtask(unsigned int set, int task)
{
    return set & (1<<task);
}

int main(void)
{
    unsigned int set = 0;

    settask(&set, 7);
    printf("set task %x\n", set);
    printf("test task %x\n", testtask(set, 7));
    clrtask(&set, 7);
    printf("clear task %x\n", set);
    printf("test task %x\n", testtask(set, 7));


    return 0;
}

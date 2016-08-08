#include <stdio.h>

int main(void)
{
    freopen("log.tmp", "a", stdout);

    printf("hello world.\n");

    return 0;
}


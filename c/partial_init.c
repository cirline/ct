#include <stdio.h>

struct s1{
    int a, b, c;
};

int main(void)
{
    struct s1 s = {.b = 3};

    printf("%d %d %d\n", s.a, s.b, s.c);

    return 0;
}



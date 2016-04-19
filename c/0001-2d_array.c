#include <stdio.h>

int main(void)
{
    int arr[2][2] = {
        {1, 2},
        {3, 4},
    };


    int *p1;
    int **p2;           // 二级指针
    int (*p)[2];        // 指针数组
    int i, j;

//    p1 = arr;
//    p2 = arr;
    p = arr;
    for(i=0; i<2; i++) {
        printf("p[%d] = %p\n", i, p[i]);
        printf("arr[%d] = %p\n", i, arr[i]);
    }

    return 0;
}



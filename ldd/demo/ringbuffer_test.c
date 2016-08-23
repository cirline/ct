#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/unistd.h>
#include <linux/fcntl.h>

#include "../chrdev/ringbuffer.h"

#define DEV_NODE    "/dev/ringbuffer"

int main(void)
{
    char buff[1024];
    int fd;
    int ret;
    int cval = 5;

    fd = open(DEV_NODE, O_RDWR);
    if(fd < 0) {
        printf("open device faied: %s\n", strerror(errno));
        return -1;
    }

    ret = ioctl(fd, RB_IOC_GET_MEMSIZE, &cval);
    printf("cval = %d\n", cval);

    return 0;

    while(strcmp(buff, "exit")) {
        printf("input string: ");
        scanf("%s", buff);
        printf("input string: %s\n", buff);

        ret = write(fd, buff, strlen(buff));
        printf("write string %d -> %d\n", (int)strlen(buff), ret);
    }

    return 0;
}



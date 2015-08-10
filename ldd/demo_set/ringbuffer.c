#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/unistd.h>
#include <linux/fcntl.h>

int main(void)
{
    char buff[1024];
    int fd;
    int ret;

    fd = open("/dev/ringb", O_RDWR);
    if(fd < 0) {
        printf("open device faied: %s\n", strerror(errno));
    }

    while(strcmp(buff, "exit")) {
        printf("input string: ");
        scanf("%s", buff);
        printf("input string: %s\n", buff);

        ret = write(fd, buff, strlen(buff));
        printf("write string %d -> %d\n", (int)strlen(buff), ret);
    }

    return 0;
}



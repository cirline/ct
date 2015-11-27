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

    while(1){
        ret = read(fd, buff, 4);
        buff[ret] = 0;
        printf("read %d char: %s\n", ret, buff);
        sleep(3);
    }

    return 0;
}



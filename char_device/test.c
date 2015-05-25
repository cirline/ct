#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    char buf[16], buf2[16];
    int ret;
    int i, j = 0;

    *buf2 = 1;

    while(1) {
        int fd = open("/dev/globalmem", O_RDWR);
        if(fd < 0) {
            printf("open failure.\n");
            return -1;
        }

        lseek(fd, 0, SEEK_SET);
        ret = read(fd, buf, 16);
        if(ret < 0) {
            printf("read error.\n");
            return -1;
        }

        printf("buf: ");
        for(i= 0; i<16; i++) {
            printf("%x ", buf[i]);
        }
        printf("\n");

        lseek(fd, j++, SEEK_SET);
        ret = write(fd, buf2, 1);
        if(ret < 0) {
            printf("write err.\n");
            return -1;
        }
        getchar();
        buf2[0] = j;
    }

    return 0;
}



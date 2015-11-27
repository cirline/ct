/**
 * list the device number: cat /proc/devices
 * create device note: mknod /dev/deviceN c major minor
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/ioctl.h>
#include <errno.h>
#include <string.h>

#define GMEM_IOC_MAGIC  'k'
#define GMEM_IOC_GET_MEMSIZE    _IOWR(GMEM_IOC_MAGIC, 0, int)

int main(void)
{
    char buf[16], buf2[16];
    int ret;
    int i, j = 0;
    int fd;
    int ctl_val;

    *buf2 = 1;

    fd = open("/dev/gmem0", O_RDWR);
    if(fd < 0) {
        printf("open gmem0 failed !!! \n");
        return -1;
    }

    ctl_val = 22;
    ret = ioctl(fd, GMEM_IOC_GET_MEMSIZE, &ctl_val);
    if(ret < 0) {
        printf("ioctl failed(%s)\n", strerror(errno));
    }
    printf("ioctl: ret =%d, ctl_val = %d\n", ret, ctl_val);

    while(1) {
        int fd = open("/dev/gmem0", O_RDWR);
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
        buf2[0] = getchar();
        while(getchar() != '\n');
    }

    return 0;
}



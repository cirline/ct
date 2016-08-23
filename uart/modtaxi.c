
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <termios.h>

#include "modtaxi.h"

#define uart_inf(format, args...)       printf(format, ##args)
#define uart_err(format, args...)       printf(format, ##args)

unsigned char calcsum(struct mtdata_desc *dp)
{
    int i, j, k = 0;
    int ret;
    unsigned char *bp;
    unsigned char sum;

    for(i = 0; i < 3; i++) {
        dp[i].valid = 0;
        bp = (unsigned char *)&dp[i];
        sum = 0xee;
        for(j = 1; bp[j] != 0xcc; j++) {
            if(bp[j+1] == 0xcc) {
                uart_inf("i = %d, curent is chksum: %x, the calcsum is: %x\n", i, bp[j], sum);
                if(bp[j] == sum) {
                    dp[i].valid = 1;
                    k++;
                }
                break;
            }
            sum ^= bp[j];
        }
    }
    return k;
}

int uart_init_taximeter(int fd)
{
    int ret;
    struct termios opt;

    bzero(&opt, sizeof(opt));
    tcgetattr(fd, &opt);
    // ex
    opt.c_cflag |= (CLOCAL | CREAD);
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_oflag &= ~OPOST;
    // set speed
    cfsetispeed(&opt, B9600);
    cfsetospeed(&opt, B9600);
    // set 8n1
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag &= ~PARENB;
    opt.c_cflag &= ~CSTOPB;
    // set timeout
    opt.c_cc[VTIME] = 150;
    opt.c_cc[VMIN] = 0;
    tcflush(fd, TCIOFLUSH);
    ret = tcsetattr(fd, TCSANOW, &opt);
    if(ret != 0) {
        uart_err("tc set attr failed: %s\n", strerror(errno));
        return -1;
    }
    tcflush(fd, TCIOFLUSH);

    return 0;
}

int recv_eecc(int fd, unsigned char *buffer)
{
    unsigned char c;
    int i, ret;

    for(i = 0, c = 0; i < 128 && c != 0xee; i++) {
        if(read(fd, &c, 1) != 1)
            return -1;
    }
    buffer[0] = 0xee;
    for(i = 1, ret = 1; ret == 1; ) {
        ret = read(fd, &c, 1);
//        uart_inf("ret = %d, c = %x\n", ret, c);
        buffer[i++] = c;
        if(c == 0xcc)
            break;
    }
    return i;
}

int uart_read_buf(struct mtdata_desc *dp)
{
    int fd;
    unsigned char c;
    int i, ret;
    int flag;
    unsigned char *bp;

    fd = open(UART_PATH, O_RDWR);
    if(fd < 0) {
        uart_err("open %s failed: %s\n", UART_PATH, strerror(errno));
        return -1;
    }
    if(uart_init_taximeter(fd) < 0) {
        uart_err("uart init failed!\n");
        return -1;
    }
    for(i = 0, ret = 0; i < 3 && ret >= 0; i++) {
        bp = (unsigned char *)(&dp[i]);
        ret = recv_eecc(fd, bp);
    }
    close(fd);

    return ret;
}


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

int main(void)
{
    int i, j, k;
    int ret;
    unsigned int uint;

    unsigned char *bp;
    struct mtdata_desc data[3];
    union mtdata_tail *dtp;

    while(1) {
        uart_inf("\nread uart: \n");
        i = uart_read_buf(data);
        ret = calcsum(data);
        uart_inf("valid data count = %d\n", ret);
        for(i = 0; i < 3; i++) {
            if(!data[i].valid)
                continue;
            dtp = &data[i].cx;
            if(data[i].command == 0xc1) {
                uart_inf("pass no = %x\n", dtp->c1.no);
                uart_inf("chksum = %x\n", dtp->c1.chksum);
            } else {
                uart_inf("pass no: %x\n", dtp->c2.no);
                uart_inf("pass num: %x\n", dtp->c2.num);
                uart_inf("send no: %x\n", dtp->c2.sendno);
                uart_inf("date: %x%x-%x-%x\n", dtp->c2.yyh, dtp->c2.yy, dtp->c2.mm, dtp->c2.dd);
                uart_inf("in time: %x:%x\n", dtp->c2.ihh, dtp->c2.imm);
                uart_inf("out time: %x:%x\n", dtp->c2.ohh, dtp->c2.omm);
                uart_inf("wait time: %x:%x:%x\n", dtp->c2.whh, dtp->c2.wmm, dtp->c2.wss);
                uart_inf("state: %x\n", dtp->c2.state);
                uint = (dtp->c2.disth << 8) | dtp->c2.distl;
                uart_inf("mileage: %x.%x\n", uint >> 4, uint & 0xf);
                uart_inf("money: %x%x:%x\n", dtp->c2.sumh, dtp->c2.suml, dtp->c2.sumd);
                uint = (dtp->c2.udisth << 8) | dtp->c2.udistl;
                uart_inf("un load mileage: %x.%x\n", uint >> 4, uint & 0xf);
                uart_inf("printer state: %x\n", dtp->c2.printer);
                uart_inf("check sum: %x\n", dtp->c2.chksum);
            }
        }
        fflush(NULL);
    }

    return 0;
}


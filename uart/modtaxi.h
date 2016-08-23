#ifndef __MODTAXI_H__
#define __MODTAXI_H__

#define UART_PATH   "/dev/ttyUSB0"

union mtdata_tail {
    struct {
        unsigned char no, chksum, cc;
    } c1;
    struct {
        unsigned char no, num, sendno;
        unsigned char yy, mm, dd;
        unsigned char ihh, imm, ohh, omm;
        unsigned char reserve1, reserve2;
        unsigned char whh, wmm, wss;
        unsigned char state;
        unsigned char disth, distl, sumh, suml, sumd;
        unsigned char udisth, udistl;
        unsigned char yyh, printer, chksum, cc;
    } c2;
};

struct mtdata_desc {
    unsigned char ee, command, length;
    union mtdata_tail cx;
    int valid;
};

extern unsigned char calcsum(struct mtdata_desc *dp);
extern int uart_init_taximeter(int fd);
extern int recv_eecc(int fd, unsigned char *data);
extern int uart_read_buf(struct mtdata_desc *dp);

#endif


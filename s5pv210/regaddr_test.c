#include <stdio.h>
#include "s5p_regs.h"

//#define UART
#define GPA

#define paddr(reg, addr)  printf("addr: 0x%x -- %s\n", (addr), reg)

int main(void)
{
#ifdef UART
    paddr("ulcon0", ULCONx(0));
    paddr("ucon0", UCONx(0));
    paddr("clcon1", ULCONx(1));
    paddr("ucon1", UCONx(1));
#endif
#ifdef GPA
    paddr("gpa0con", GPAxCON(0));
#endif
    return 0;
}




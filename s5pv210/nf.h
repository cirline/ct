#ifndef __NANDFLASH_H__
#define __NANDFLASH_H__

#include "s5p_regs.h"

#define NFCMD_RESET			0xFF
#define NFCMD_READ1			0x00
#define NFCMD_READ2			0x30
#define NFCMD_ERASE1		0x60
#define NFCMD_ERASE2		0xD0
#define NFCMD_PAGE_PRO1		0x80
#define NFCMD_PAGE_PRO2		0x10
#define NFCMD_READID		0x90
#define NFCMD_READSTATUS	0x70

#define NF_PAGESIZE			2048

extern int nf_init(void);
extern void inline nf_reset(void);
extern int nf_read(const int addr, unsigned char *buf);
extern int nf_erase(unsigned long addr);
extern int nf_write(unsigned long addr, unsigned char *buf);
#endif

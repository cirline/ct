#include "common.h"
#include "s5p_regs.h"
#include "nf.h"

void inline nf_ncs0(int x)
{
	region_write(NFCONT, 0x1, 1, (x));
}

void inline nf_clear_rb(void)
{
	region_write(NFSTAT, 0x1, 4, 1);
}

void inline nf_detect_rb(void)
{
	while(!((1<<4) & __raw_read(NFSTAT)));
}

void inline nf_wait_rb(void)
{
	while(!((1<<0) & __raw_read(NFSTAT)));
}

void inline nf_cmd(unsigned char c)
{
	__raw_write(NFCMMD, c);
}

void inline nf_setaddr(unsigned char c)
{
	__raw_write(NFADDR, c);
}

unsigned char inline nf_data(void)
{
	return (__raw_read(NFDATA) & 0xff);
}

void inline nf_wait_io(void)
{
	while((1) & nf_data());
}

int nf_init(void)
{
	fstart();
	__raw_write(MP0_xCON(6), 0x22222222);		// IO BUS
	region_write(MP0_xCON(3), 0xf, 16, 0x2);	// RB
	region_write(MP0_xCON(3), 0xf, 12, 0x2);	// RE
	region_write(MP0_xCON(1), 0xf, 8, 0x3);		// CS
	region_write(MP0_xCON(3), 0xf, 0, 0x2);		// CLE
	region_write(MP0_xCON(3), 0xf, 4, 0x2);		// ALE
	region_write(MP0_xCON(3), 0xf, 8, 0x2);		// WE

	region_write(NFCONF, 0xfff, 4, 0xfff);		// TACLS, TWRPH0, TWRPH1
	region_write(NFCONF, 0xf, 0, (1<1));		// 5 cycle
	region_write(NFCONT, 0x1, 0, 0);
	region_write(NFCONT, 0x1, 0, 1);			// enable nf
	fend();
	return 0;
}

void inline nf_reset(void)
{
	fstart();
	nf_ncs0(0);
	nf_cmd(NFCMD_RESET);
	nf_wait_rb();
	nf_ncs0(1);
	fend();
}

int nf_read(const int addr, unsigned char *buf)
{
	int i;
	int page = addr >> 11;

	fstart();
	nf_reset();
	nf_ncs0(0);
	nf_cmd(NFCMD_READ1);
	nf_setaddr(0);
	nf_setaddr(0);
	nf_setaddr(page & 0xff);
	nf_setaddr((page>>8) & 0xff);
	nf_setaddr((page>>16) & 0xff);
	nf_cmd(NFCMD_READ2);

	nf_wait_rb();

	for(i=0; i<NF_PAGESIZE; i++) {
		buf[i] = nf_data();
	}

	nf_ncs0(1);
	fend();
	return 0;
}

int nf_erase(unsigned long addr)
{
	fstart();
	addr >>= 17;
	nf_reset();
	nf_ncs0(0);
	nf_cmd(NFCMD_ERASE1);
	nf_setaddr(addr & 0xff);
	nf_setaddr((addr>>8) & 0xff);
	nf_setaddr((addr>>16) & 0xff);
	nf_cmd(NFCMD_ERASE2);
	nf_wait_rb();
	nf_cmd(NFCMD_READSTATUS);
	nf_wait_io();
	nf_ncs0(1);

	fend();
	return 0;
}

int nf_write(unsigned long addr, unsigned char *buf)
{
	int i;
	int page = addr >> 11;

	nf_ncs0(0);
	nf_clear_rb();
	nf_cmd(NFCMD_PAGE_PRO1);
	nf_setaddr(0);
	nf_setaddr(0);
	nf_setaddr(page & 0xff);
	nf_setaddr((page>>8) & 0xff);
	nf_setaddr((page>>16) & 0xff);

	for(i=0; i<NF_PAGESIZE; i++) {
		__raw_write(NFDATA, buf[i]);
	}
	nf_cmd(NFCMD_PAGE_PRO2);
	nf_wait_rb();
	nf_ncs0(1);
	return 0;
}

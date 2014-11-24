#include "s5p_regs.h"
#include "common.h"
#include "i2c.h"

static void inline i2c_ack_toggle(int x) { region_write(I2CCONx(0), 0x1, 7, x); }

static void inline i2c_set_mode(i2c_mode_t mode) { region_write(I2CSTATx(0), 0x3, 6, mode); }

static void inline i2c_start(void) { region_write(I2CSTATx(0), 0x1, 5, 1); }

static void inline i2c_stop(void) { region_write(I2CSTATx(0), 0x1, 5, 0); }

/* 0 = not busy, 1 = busy */
static int inline i2c_state(void) { return __raw_read(I2CSTATx(0)) & (1<<5); }

static int inline i2c_trx_pend(void) { return __raw_read(I2CCONx(0)) & (1<<4); }

static void inline i2c_trx_resume(void) { region_write(I2CCONx(0), 0x1, 4, 0); }

/* 0 = I2CADD writable, 1 = I2CDS writable */
static void inline i2c_trx_toggle(int x) { region_write(I2CSTATx(0), 0x1, 4, x); }

static int inline i2c_not_rcv_ack(void) { return (__raw_read(I2CSTATx(0)) & 0x1); }

static void inline i2c_data_write(int ch) { __raw_write(I2CDSx(0), (ch & 0xff)); }

static char inline i2c_data_read(void) { return __raw_read(I2CDSx(0)) & 0xff; }

int i2c_init(void)
{
	region_write(GPDxCON(1), 0xff, 0, 0x22);	// set gpio
	set2clear(VICxINTENCLEAR(1), 0x1, 14);		// disable i2c irq
	region_write(I2CCONx(0), 0x1, 6, 1);		// clk src
	region_write(I2CCONx(0), 0x1, 5, 1);		// tx/rx interrupt
	region_write(I2CCONx(0), 0xf, 0, 0xffff);	// tx clk val
	return 0;
}

static int i2c_select_device(unsigned char addr, int rw_flag)
{
	int try;
	for(try = 0; try < I2C_MAX_TRY; try++) {
		i2c_data_write((addr<<1) | rw_flag);
		i2c_ack_toggle(I2C_SET_ACK);
		i2c_trx_resume();
		i2c_set_mode((rw_flag == I2C_READ) ? MASTER_RX : MASTER_TX);
		i2c_trx_toggle(1);
		i2c_start();
		while(!i2c_trx_pend());
		if(!i2c_not_rcv_ack())
			return 0;
		i2c_stop();
	}
	return -1;
}

static int inline i2c_send_addr(unsigned char addr)
{
	i2c_data_write(addr);
	i2c_trx_resume();
	while(!i2c_trx_pend());
	return i2c_not_rcv_ack();
}

static int inline i2c_restart(unsigned char addr, int rw_flag)
{
	i2c_data_write((addr<<1) | rw_flag);
	i2c_set_mode((rw_flag == I2C_READ) ? MASTER_RX : MASTER_TX);
	i2c_start();
	i2c_trx_resume();
	while(!i2c_trx_pend());
	return i2c_not_rcv_ack();
}

static int inline i2c_receive_data(int ack, char *ch)
{
	i2c_ack_toggle(ack);
	i2c_trx_resume();
	while(!i2c_trx_pend());
	*ch = i2c_data_read();
	return i2c_not_rcv_ack();
}

int i2c_read_byte(unsigned char addr, char *ch)
{
	return i2c_read_array(addr, ch, 1);
}

int i2c_read_array(unsigned char addr, char *buf, int len)
{
	int ret;
	int i;

	ret = i2c_select_device(0x50, I2C_WRITE);
	if(ret) {
		printf("i2c devices no response.\n");
		goto i2c_select_device_fail;
	}
	ret = i2c_send_addr(addr);
	if(ret) {
		printf("i2c send address fail.\n");
		goto i2c_fail;
	}
	ret = i2c_restart(0x50, I2C_READ);
	if(ret) {
		printf("i2c restart fail.\n");
		goto i2c_fail;
	}
	for(i = 0; i < len-1; i++) {
		ret = i2c_receive_data(I2C_SET_ACK, buf + i);
		if(ret) {
			printf("i2c receive data fail, 0x%p bytes success\n", i);
			goto i2c_fail;
		}
	}
	i2c_receive_data(I2C_SET_NACK, buf + i);
	ret = len;
i2c_fail:
	i2c_stop();
	i2c_trx_resume();
i2c_select_device_fail:
	return ret;
}



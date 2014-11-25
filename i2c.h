#ifndef __I2C_H__
#define __I2C_H__

#include "s5p_regs.h"

#define I2C_MAX_TRY		3
#define I2C_READ		1
#define I2C_WRITE		0
#define I2C_SET_NACK	(0<<0)
#define I2C_SET_ACK		(1<<0)

typedef enum i2c_mode {
	SLAVE_RX = 0,
	SLAVE_TX,
	MASTER_RX,
	MASTER_TX
} i2c_mode_t;

extern int i2c_init(void);
extern int i2c_read_byte(unsigned char addr, char *ch);
extern int i2c_read_array(unsigned char addr, char *buf, int len);
extern int i2c_write_byte(unsigned char addr, char ch);
extern int i2c_write_array(unsigned char addr, char *buf, int len);


#endif


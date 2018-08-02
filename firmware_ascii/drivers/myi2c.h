#ifndef _MYI2C_H_
#define _MYI2C_H_

static void delay(void);
void i2c_start(void);
void i2c_stop(void);
static void I2C_Ack(void);
static void i2c_write_byte(u8 u8WriteData);
static u8  i2c_read_byte(void);
static void send_SDA_Data(u8 u8WriteData);
static u8 receive_SDA_Data(void);
u8 i2c_read(u8 u8SlaveAddr, u8 u8ReadAddr);
void i2c_write(u8 u8SlaveAddr, u8 u8WriteData, u8 u8ReadAddr);

#endif
/* This file is used to save the chinese data */

#include "configuration.h"

extern u8 au8GB2312WordArray[50][LINE_MAX_NUMBER*2];
extern bool bWriteIsOn;
extern u8 u8WordNumber;

u8 au8GetDataFromFM24CL16B[50][LINE_MAX_NUMBER*2];

/* 此函数用来向存储芯片中写入,读取数据 */
void read_out_data(void)
{
  u8 u8StoreAddress = 0;
  u8 u8PageNumber = 0;
  u8 u8SlaveAddress = 0xA0;
  
  AT91C_BASE_PIOA->PIO_PER = PA_09_I2C_SDA;
  AT91C_BASE_PIOA->PIO_PER = PA_10_I2C_SCL;
  
  for (u8 i=0; i<50; i++)
  {
    for (u8 j=0; j<LINE_MAX_NUMBER*2; j++)
    {
      if (bWriteIsOn)
      {
        i2c_write(u8SlaveAddress, au8GB2312WordArray[i][j], u8StoreAddress);
        /* 写入汉字个数 */
        i2c_write(u8SlaveAddress|0x0E, u8WordNumber, 0x00);
      }
      au8GetDataFromFM24CL16B[i][j] = i2c_read(u8SlaveAddress, u8StoreAddress);
      
      u8StoreAddress++;
      if (u8StoreAddress == 0xFF)
      {
        u8StoreAddress = 0;
        u8PageNumber++;
        u8SlaveAddress = u8SlaveAddress | (u8PageNumber<<1);
      }
    }
  }
  
  u8WordNumber = i2c_read(u8SlaveAddress|0x0E, 0x00);
}

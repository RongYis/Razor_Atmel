/* This File is used to create my own i2C */

#include "configuration.h"

static void delay(void)
{
  for (u8 i=0; i<5; i++);
}

/* I2C起始条件 */  
void i2c_start(void)  
{  
  //初始化GPIO口  
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;         //设置SDA方向为输出  
  AT91C_BASE_PIOA->PIO_OER = PA_10_I2C_SCL;         //设置SCL方向为输出  
  AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;        //设置SDA为高电平  
  AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;        //设置SCL为高电平  
  delay();                                          //延时  
  
  //起始条件  
  AT91C_BASE_PIOA->PIO_CODR = PA_09_I2C_SDA;        //SCL为高电平时，SDA由高变低  
  delay();                                          //延时   
}

/* I2C终止条件 */  
void i2c_stop(void)  
{     
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;         //设置SDA方向为输出  
  AT91C_BASE_PIOA->PIO_OER = PA_10_I2C_SCL;         //设置SCL方向为输出 
  AT91C_BASE_PIOA->PIO_CODR = PA_09_I2C_SDA;        //设置SDA为低电平
  AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;        //设置SCL为高电平
     
  delay();                                          //延时   
  AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;        //SCL为高电平时，SDA由低变高 
  delay();                                          //延时
}  


/*   
I2C读取ACK信号(写数据时使用)  
返回值 ：0表示ACK信号有效；非0表示ACK信号无效  
*/  
static void I2C_Ack(void)
{
  u8 i = 0;
  AT91C_BASE_PIOA->PIO_ODR = PA_09_I2C_SDA;         //设置SDA方向为输入
  AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;        //设置SCL为高电平
  delay();                                          //延时
  
  while ((receive_SDA_Data()==1) && (i<255))
    i++;
  
  AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;        //设置SCL为低电平
  delay();                                          //延时
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;         //设置SDA方向为输出 
}

/* I2C字节写 */  
static void i2c_write_byte(u8 u8WriteData)  
{    
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;          //设置SDA方向为输出 
  
  for (u8 i=0; i<8; i++) 
  { 
    AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;       // SCL变低  
    delay();                                          //延时  
    
    send_SDA_Data(u8WriteData);                    //从高位到低位依次准备数据进行发送 
    u8WriteData = u8WriteData<<1;
    
    AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;       // SCL变高  
    delay();                                          //延时   
  }
  
  AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;       // SCL变低
  delay();                                          //延时
  AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;       // SDA变高  
  delay();                                          //延时  
}


/* I2C字节读 */  
static u8 i2c_read_byte(void)  
{    
  u8 u8ReceiveData = 0;  
  AT91C_BASE_PIOA->PIO_ODR = PA_09_I2C_SDA;          //设置SDA方向为输入
  delay();
  
  for (u8 i=0; i<8; i++) 
  {  
    AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;       // SCL变高  
    delay(); 
    
    u8ReceiveData |= (receive_SDA_Data() << (7-i));      //从高位到低位依次准备数据进行读取 
    
    AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;       // SCL变低  
    delay();  
  }
  
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;          //设置SDA方向为输出  
  return u8ReceiveData; 
}

/*  
I2C读操作  
addr：目标设备地址  
buf：读缓冲区  
len：读入字节的长度  
*/  
u8 i2c_read(u8 u8SlaveAddr, u8 u8ReadAddr)  
{
  u8 u8Data = 0;
  u8 u8CurrentAddr = 0;
  i2c_start();                        //起始条件，开始数据通信  
  
  i2c_write_byte(u8SlaveAddr);
  I2C_Ack();
  i2c_write_byte(u8ReadAddr);
  I2C_Ack();
  i2c_start();                        //起始条件，开始数据通信  
  //发送地址和数据读写方向  
  u8CurrentAddr = u8SlaveAddr | 0x01;                    //低位为1，表示读数据 
  
  i2c_write_byte(u8CurrentAddr);  
  I2C_Ack();
  
  //读入数据  
  u8Data = i2c_read_byte();  
  
  i2c_stop();                     //终止条件，结束数据通信  
  return u8Data;
}


/*  
I2C写操作  
addr：目标设备地址  
buf：写缓冲区  
len：写入字节的长度  
*/  
void i2c_write(u8 u8SlaveAddr, u8 u8WriteData, u8 u8ReadAddr)  
{  
  u8 u8CurrentAddr = 0;
  i2c_start();                        //起始条件，开始数据通信  
  
  //发送地址和数据读写方向  
  u8CurrentAddr = u8SlaveAddr | 0x00;                   //低位为0，表示写数据 
  
  i2c_write_byte(u8CurrentAddr); 
  I2C_Ack();
  i2c_write_byte(u8ReadAddr);
  I2C_Ack();
  
  //写入数据  
  i2c_write_byte(u8WriteData); 
  I2C_Ack();
  
  i2c_stop();                     //终止条件，结束数据通信  
}


/* 用来发送SDA数据 */
static void send_SDA_Data(u8 u8WriteData)
{
  if ((u8WriteData & 0x80))
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;    //SDA-----1
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_09_I2C_SDA;    //SDA-----0
  }
}


/* 用来读取SDA数据 */
static u8 receive_SDA_Data(void)
{
  static u8 u8GetEachBit = 0;
  
  /* 判断收的每一位是0还是1 */
  if ((AT91C_BASE_PIOA->PIO_PDSR & PA_09_I2C_SDA))  
  {
    u8GetEachBit = 1;
  }
  else
  {
    u8GetEachBit = 0;
  }
  
  return u8GetEachBit;
}

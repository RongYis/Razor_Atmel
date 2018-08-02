/* This File is used to create my own i2C */

#include "configuration.h"

static void delay(void)
{
  for (u8 i=0; i<5; i++);
}

/* I2C��ʼ���� */  
void i2c_start(void)  
{  
  //��ʼ��GPIO��  
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;         //����SDA����Ϊ���  
  AT91C_BASE_PIOA->PIO_OER = PA_10_I2C_SCL;         //����SCL����Ϊ���  
  AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;        //����SDAΪ�ߵ�ƽ  
  AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;        //����SCLΪ�ߵ�ƽ  
  delay();                                          //��ʱ  
  
  //��ʼ����  
  AT91C_BASE_PIOA->PIO_CODR = PA_09_I2C_SDA;        //SCLΪ�ߵ�ƽʱ��SDA�ɸ߱��  
  delay();                                          //��ʱ   
}

/* I2C��ֹ���� */  
void i2c_stop(void)  
{     
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;         //����SDA����Ϊ���  
  AT91C_BASE_PIOA->PIO_OER = PA_10_I2C_SCL;         //����SCL����Ϊ��� 
  AT91C_BASE_PIOA->PIO_CODR = PA_09_I2C_SDA;        //����SDAΪ�͵�ƽ
  AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;        //����SCLΪ�ߵ�ƽ
     
  delay();                                          //��ʱ   
  AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;        //SCLΪ�ߵ�ƽʱ��SDA�ɵͱ�� 
  delay();                                          //��ʱ
}  


/*   
I2C��ȡACK�ź�(д����ʱʹ��)  
����ֵ ��0��ʾACK�ź���Ч����0��ʾACK�ź���Ч  
*/  
static void I2C_Ack(void)
{
  u8 i = 0;
  AT91C_BASE_PIOA->PIO_ODR = PA_09_I2C_SDA;         //����SDA����Ϊ����
  AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;        //����SCLΪ�ߵ�ƽ
  delay();                                          //��ʱ
  
  while ((receive_SDA_Data()==1) && (i<255))
    i++;
  
  AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;        //����SCLΪ�͵�ƽ
  delay();                                          //��ʱ
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;         //����SDA����Ϊ��� 
}

/* I2C�ֽ�д */  
static void i2c_write_byte(u8 u8WriteData)  
{    
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;          //����SDA����Ϊ��� 
  
  for (u8 i=0; i<8; i++) 
  { 
    AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;       // SCL���  
    delay();                                          //��ʱ  
    
    send_SDA_Data(u8WriteData);                    //�Ӹ�λ����λ����׼�����ݽ��з��� 
    u8WriteData = u8WriteData<<1;
    
    AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;       // SCL���  
    delay();                                          //��ʱ   
  }
  
  AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;       // SCL���
  delay();                                          //��ʱ
  AT91C_BASE_PIOA->PIO_SODR = PA_09_I2C_SDA;       // SDA���  
  delay();                                          //��ʱ  
}


/* I2C�ֽڶ� */  
static u8 i2c_read_byte(void)  
{    
  u8 u8ReceiveData = 0;  
  AT91C_BASE_PIOA->PIO_ODR = PA_09_I2C_SDA;          //����SDA����Ϊ����
  delay();
  
  for (u8 i=0; i<8; i++) 
  {  
    AT91C_BASE_PIOA->PIO_SODR = PA_10_I2C_SCL;       // SCL���  
    delay(); 
    
    u8ReceiveData |= (receive_SDA_Data() << (7-i));      //�Ӹ�λ����λ����׼�����ݽ��ж�ȡ 
    
    AT91C_BASE_PIOA->PIO_CODR = PA_10_I2C_SCL;       // SCL���  
    delay();  
  }
  
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_SDA;          //����SDA����Ϊ���  
  return u8ReceiveData; 
}

/*  
I2C������  
addr��Ŀ���豸��ַ  
buf����������  
len�������ֽڵĳ���  
*/  
u8 i2c_read(u8 u8SlaveAddr, u8 u8ReadAddr)  
{
  u8 u8Data = 0;
  u8 u8CurrentAddr = 0;
  i2c_start();                        //��ʼ��������ʼ����ͨ��  
  
  i2c_write_byte(u8SlaveAddr);
  I2C_Ack();
  i2c_write_byte(u8ReadAddr);
  I2C_Ack();
  i2c_start();                        //��ʼ��������ʼ����ͨ��  
  //���͵�ַ�����ݶ�д����  
  u8CurrentAddr = u8SlaveAddr | 0x01;                    //��λΪ1����ʾ������ 
  
  i2c_write_byte(u8CurrentAddr);  
  I2C_Ack();
  
  //��������  
  u8Data = i2c_read_byte();  
  
  i2c_stop();                     //��ֹ��������������ͨ��  
  return u8Data;
}


/*  
I2Cд����  
addr��Ŀ���豸��ַ  
buf��д������  
len��д���ֽڵĳ���  
*/  
void i2c_write(u8 u8SlaveAddr, u8 u8WriteData, u8 u8ReadAddr)  
{  
  u8 u8CurrentAddr = 0;
  i2c_start();                        //��ʼ��������ʼ����ͨ��  
  
  //���͵�ַ�����ݶ�д����  
  u8CurrentAddr = u8SlaveAddr | 0x00;                   //��λΪ0����ʾд���� 
  
  i2c_write_byte(u8CurrentAddr); 
  I2C_Ack();
  i2c_write_byte(u8ReadAddr);
  I2C_Ack();
  
  //д������  
  i2c_write_byte(u8WriteData); 
  I2C_Ack();
  
  i2c_stop();                     //��ֹ��������������ͨ��  
}


/* ��������SDA���� */
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


/* ������ȡSDA���� */
static u8 receive_SDA_Data(void)
{
  static u8 u8GetEachBit = 0;
  
  /* �ж��յ�ÿһλ��0����1 */
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

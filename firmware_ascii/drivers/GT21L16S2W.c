/*  this file is used to get chinese character from GT21L16S2W */

#include "configuration.h"

extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */

u8 au8GB2312WordArray[50][LINE_MAX_NUMBER*2];
u8 u8WordNumber = 0;

/* 从Debug中获取中文 */
bool GetWord_From_Debug(void)
{
  static u8 UserApp_au8UserInputBuffer[100];
  static u16 u16Chinese[50];
   
  if (G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1] == 0x0A)
  {
    u8WordNumber = DebugScanf(UserApp_au8UserInputBuffer)-2;    //从串口传入中文数据

    for (u8 i=0; i<50; i++)
    {
      u16Chinese[i] = (UserApp_au8UserInputBuffer[2*i]<<8) | UserApp_au8UserInputBuffer[2*i+1];
    }
      
    GetData_From_GT21L16S2W(u16Chinese);
    
    return TRUE;
  }
  else
  {
    return FALSE;
  }
      
}


/* 此函数可获取 n 个汉字的信息 */
void GetData_From_GT21L16S2W(u16* u16Word)
{
  static u8* pau8GetOneWord = NULL;
  static u16 u16OneWord = 0;
  
  for (u8 i=0; i<(u8WordNumber/2); i++)
  {
    for (u8 j=0; j<LINE_MAX_NUMBER*2; j++)
    {
      au8GB2312WordArray[i][j] = 0;
    }
  }
  
  for (u8 i=0; i<(u8WordNumber/2); i++)
  {
    u16OneWord = u16Word[i];
    pau8GetOneWord = Get_One_Word(u16OneWord);
    for (u8 j=0; j<LINE_MAX_NUMBER*2; j++)
    {
      au8GB2312WordArray[i][j] = *(pau8GetOneWord+j);
    }
  }
}

/* 此函数得到汉字在 GB2312 的字节地址 */
static u32 Get_Word_Address(u16 u16OneWord)
{
  static u32 u32AddressData = 0;
  static u8 u8MSB = 0;
  static u8 u8LSB = 0;
  
  u8MSB = (u8)(u16OneWord>>8 & 0xFF);     //获取汉字内码高8位
  u8LSB = (u8)(u16OneWord>>0 & 0xFF);     //获取汉字内码低8位
  
  /* 计算点阵汉字地址 */
  if ((u8MSB>=0xA4 && u8MSB<=0xA8) && (u8LSB>=0xA1))
  {
    u32AddressData = BASEADDRESS;
  }
  else if ((u8MSB>=0xA1 && u8MSB<=0xA9) && (u8LSB>=0xA1))
  {
    u32AddressData = ((u8MSB-0xA1)*94 + (u8LSB-0xA1))*32 + BASEADDRESS;
  }
  else if ((u8MSB>=0xB0 && u8MSB<=0xF7) && (u8LSB>=0xA1))
  {
    u32AddressData = ((u8MSB-0xB0)*94 + (u8LSB-0xA1) + 846)*32 + BASEADDRESS;
  }
  
  return u32AddressData;
}

/* 此函数用来给字库芯片发送指令和地址信息数据 */
static void Send_SI_BYTE(u8 u8SendData)
{   
  for (u8 i=0; i<ONE_BYTE_TO_BITS; i++)  //处理1个字节数据
  {
    AT91C_BASE_PIOB->PIO_CODR = PB_07_TP60;  //SCLK-----0
    
    Send_IN_EACH_BIT(u8SendData);  //发送一位数据到SI
    u8SendData = u8SendData<<1;
    
    /* 产生上升沿SCLK */
    AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;  //SCLK-----1
  }
}

/* 此函数用来发送每一位的数据(即控制SI的高低电平) */
static void Send_IN_EACH_BIT(u8 u8SendData)
{
  if ((u8SendData&BYTE_MAX_BIT))  //扫描最后一位是否为高电平
  {
    AT91C_BASE_PIOB->PIO_SODR = PB_08_TP62;  //SI-----1
  }
  else
  {
    AT91C_BASE_PIOB->PIO_CODR = PB_08_TP62;  //SI-----0
  }
}

/* 此函数用来获取字库芯片的一个汉字数据 */
static u8* Get_One_Word(u16 u16OneWord)
{
  static u32 u32SendSIData = 0;          
  static u8 u8Cmd = GET_WORD_CMD;
  static u8 u8AddressData = 0;
  static u8 au8GetChineseWord[ONE_WORD_BYTES];
  
  /* 读取每个字之前的初始化配置 */
  AT91C_BASE_PIOB->PIO_SODR = PB_05_TP56;      //CS-----1
  AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;      //SCLK-----1
  AT91C_BASE_PIOB->PIO_SODR = PB_08_TP62;      //SI-----1  
  AT91C_BASE_PIOB->PIO_CODR = PB_05_TP56;      //CS-----0
  AT91C_BASE_PIOB->PIO_CODR = PB_07_TP60;      //SCLK-----0
 
  u32SendSIData = Get_Word_Address(u16OneWord);  //获取汉字内码地址
  
  Send_SI_BYTE(u8Cmd);       //给字库芯片发送指令
  
  /* 存放 16-23  15-8  7-0 位的地址并发送 */
  for (u8 i=ADDRESS_NUMBERS; i>0; i--)
  {
    u8AddressData =((u8)(u32SendSIData>>(8*(i-1)))&0xFF);  //存放地址
    Send_SI_BYTE(u8AddressData);                           //发送地址信息
  }
  
  /* 得到一个字的32个字节数据 */
  for (u8 i=0; i<ONE_WORD_BYTES; i++)                //一个字要32个字节的数据
  {
    au8GetChineseWord[i] = Get_Each_Byte();          //得到每个字节的数据
  }
  
  AT91C_BASE_PIOB->PIO_SODR = PB_05_TP56;            //CS-----1
  
  return au8GetChineseWord;
}

/* 此函数用来获取中文汉字数据信息 */
static u8 Get_Each_Byte(void)
{
  u8 u8GetChineseData = 0x00;              //每次都初始化
  
  AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;  //SCLK-----1
  
  for (u8 i=0; i<ONE_BYTE_TO_BITS; i++)                   //获得一个字节的信息
  {
    /* 产生下降沿SCLK */
    AT91C_BASE_PIOB->PIO_CODR = PB_07_TP60;    //SCLK-----0
 
    u8GetChineseData |= (Get_Each_Bit() << (7-i)); //得到每一位的数据
    
    AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;    //SCLK-----1
  }
  
  return u8GetChineseData;
}


/* 此函数用来获得每一位的数据 */
static u8 Get_Each_Bit(void)
{
  static u8 u8GetEachBit = 0;
  
  /* 判断收的每一位是0还是1 */
  if ((AT91C_BASE_PIOB->PIO_PDSR & PB_06_TP58))  
  {
    u8GetEachBit = 1;
  }
  else
  {
    u8GetEachBit = 0;
  }
  
  return u8GetEachBit;
}

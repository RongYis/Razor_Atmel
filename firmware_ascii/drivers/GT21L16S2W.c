/*  this file is used to get chinese character from GT21L16S2W */

#include "configuration.h"

extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */

u8 au8GB2312WordArray[50][LINE_MAX_NUMBER*2];
u8 u8WordNumber = 0;

/* ��Debug�л�ȡ���� */
bool GetWord_From_Debug(void)
{
  static u8 UserApp_au8UserInputBuffer[100];
  static u16 u16Chinese[50];
   
  if (G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1] == 0x0A)
  {
    u8WordNumber = DebugScanf(UserApp_au8UserInputBuffer)-2;    //�Ӵ��ڴ�����������

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


/* �˺����ɻ�ȡ n �����ֵ���Ϣ */
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

/* �˺����õ������� GB2312 ���ֽڵ�ַ */
static u32 Get_Word_Address(u16 u16OneWord)
{
  static u32 u32AddressData = 0;
  static u8 u8MSB = 0;
  static u8 u8LSB = 0;
  
  u8MSB = (u8)(u16OneWord>>8 & 0xFF);     //��ȡ���������8λ
  u8LSB = (u8)(u16OneWord>>0 & 0xFF);     //��ȡ���������8λ
  
  /* ��������ֵ�ַ */
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

/* �˺����������ֿ�оƬ����ָ��͵�ַ��Ϣ���� */
static void Send_SI_BYTE(u8 u8SendData)
{   
  for (u8 i=0; i<ONE_BYTE_TO_BITS; i++)  //����1���ֽ�����
  {
    AT91C_BASE_PIOB->PIO_CODR = PB_07_TP60;  //SCLK-----0
    
    Send_IN_EACH_BIT(u8SendData);  //����һλ���ݵ�SI
    u8SendData = u8SendData<<1;
    
    /* ����������SCLK */
    AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;  //SCLK-----1
  }
}

/* �˺�����������ÿһλ������(������SI�ĸߵ͵�ƽ) */
static void Send_IN_EACH_BIT(u8 u8SendData)
{
  if ((u8SendData&BYTE_MAX_BIT))  //ɨ�����һλ�Ƿ�Ϊ�ߵ�ƽ
  {
    AT91C_BASE_PIOB->PIO_SODR = PB_08_TP62;  //SI-----1
  }
  else
  {
    AT91C_BASE_PIOB->PIO_CODR = PB_08_TP62;  //SI-----0
  }
}

/* �˺���������ȡ�ֿ�оƬ��һ���������� */
static u8* Get_One_Word(u16 u16OneWord)
{
  static u32 u32SendSIData = 0;          
  static u8 u8Cmd = GET_WORD_CMD;
  static u8 u8AddressData = 0;
  static u8 au8GetChineseWord[ONE_WORD_BYTES];
  
  /* ��ȡÿ����֮ǰ�ĳ�ʼ������ */
  AT91C_BASE_PIOB->PIO_SODR = PB_05_TP56;      //CS-----1
  AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;      //SCLK-----1
  AT91C_BASE_PIOB->PIO_SODR = PB_08_TP62;      //SI-----1  
  AT91C_BASE_PIOB->PIO_CODR = PB_05_TP56;      //CS-----0
  AT91C_BASE_PIOB->PIO_CODR = PB_07_TP60;      //SCLK-----0
 
  u32SendSIData = Get_Word_Address(u16OneWord);  //��ȡ���������ַ
  
  Send_SI_BYTE(u8Cmd);       //���ֿ�оƬ����ָ��
  
  /* ��� 16-23  15-8  7-0 λ�ĵ�ַ������ */
  for (u8 i=ADDRESS_NUMBERS; i>0; i--)
  {
    u8AddressData =((u8)(u32SendSIData>>(8*(i-1)))&0xFF);  //��ŵ�ַ
    Send_SI_BYTE(u8AddressData);                           //���͵�ַ��Ϣ
  }
  
  /* �õ�һ���ֵ�32���ֽ����� */
  for (u8 i=0; i<ONE_WORD_BYTES; i++)                //һ����Ҫ32���ֽڵ�����
  {
    au8GetChineseWord[i] = Get_Each_Byte();          //�õ�ÿ���ֽڵ�����
  }
  
  AT91C_BASE_PIOB->PIO_SODR = PB_05_TP56;            //CS-----1
  
  return au8GetChineseWord;
}

/* �˺���������ȡ���ĺ���������Ϣ */
static u8 Get_Each_Byte(void)
{
  u8 u8GetChineseData = 0x00;              //ÿ�ζ���ʼ��
  
  AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;  //SCLK-----1
  
  for (u8 i=0; i<ONE_BYTE_TO_BITS; i++)                   //���һ���ֽڵ���Ϣ
  {
    /* �����½���SCLK */
    AT91C_BASE_PIOB->PIO_CODR = PB_07_TP60;    //SCLK-----0
 
    u8GetChineseData |= (Get_Each_Bit() << (7-i)); //�õ�ÿһλ������
    
    AT91C_BASE_PIOB->PIO_SODR = PB_07_TP60;    //SCLK-----1
  }
  
  return u8GetChineseData;
}


/* �˺����������ÿһλ������ */
static u8 Get_Each_Bit(void)
{
  static u8 u8GetEachBit = 0;
  
  /* �ж��յ�ÿһλ��0����1 */
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

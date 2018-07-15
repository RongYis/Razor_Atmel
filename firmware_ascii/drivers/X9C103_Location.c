/*This file is used to display the X9C103 location*/

#include "configuration.h"

u16 u16ADCIndex = 0;

extern s8 s8VoiceIndex;

/*Function to Display the 103 location*/
void Display103_Location(void)
{
  static u8 au8ADCIndex[20] = "X9C103 LOCATION:   %";
  
  /*Get the value of A/D*/
  for (u16 i=0; i<10000; i++)
  {
    Adc12StartConversion(ADC12_CH2);
    u16ADCIndex = AT91C_BASE_ADC12B->ADC12B_CDR[ADC12_CH2];  
  }

  /*calculate the percentage of the full location*/
  u16ADCIndex = (u16)(u16ADCIndex * 100 / 0xFFF);
  
  au8ADCIndex[16] = HexToASCIICharLower((u8)u16ADCIndex/100);
  au8ADCIndex[17] = HexToASCIICharLower((u8)u16ADCIndex/10%10);
  au8ADCIndex[18] = HexToASCIICharLower((u8)u16ADCIndex%10);
  
  LCDMessage(LINE1_START_ADDR, au8ADCIndex);
}

void Display_Voice(void)
{
  static u8 au8VoiceIndex[20] = "Voice   : 000 %   ";
  static u8 u8VoiceIndex = 0;
  
  /* get the Current Voice */
  u8VoiceIndex = s8VoiceIndex;
  
  au8VoiceIndex[10] = HexToASCIICharLower(u8VoiceIndex/100);
  au8VoiceIndex[11] = HexToASCIICharLower(u8VoiceIndex/10%10);
  au8VoiceIndex[12] = HexToASCIICharLower(u8VoiceIndex%10);
  
  LCDMessage(LINE1_START_ADDR, au8VoiceIndex);
}
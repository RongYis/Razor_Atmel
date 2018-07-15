/* This file is for user to modify the voice */

#include "configuration.h"

s8 s8VoiceIndex = 0;

extern bool bInADCMode;
extern bool bInSilent;

void IncreaseVoice(void)
{
  /*The RED LED shitf when Press BUTTON3*/
  if (IsButtonPressed(BUTTON0))
  {
    LedOn(RED);
  }
  else
  {
    LedOff(RED);
  }
  
  /* Press BUTTON0 to increase the voice */
  if (WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_16_BLADE_CS;  // clear /CS bit
    AT91C_BASE_PIOA->PIO_SODR |= PA_14_BLADE_MOSI;  // set U/D bit to increase
    
    /* realize an up-to-down signal */
    for (u16 i=0; i<10; i++)
    {
      AT91C_BASE_PIOA->PIO_SODR |= PA_12_BLADE_UPOMI; // set /INC bit 
      for (u16 j=0; j<1000; j++);
      AT91C_BASE_PIOA->PIO_CODR |= PA_12_BLADE_UPOMI; // clear /INC bit 
    }
    
    s8VoiceIndex += 10;
    if (s8VoiceIndex > 100)
    {
      s8VoiceIndex = 100;
    }
    
    if (bInADCMode)  //Mode: Display X9C103 Location
    {
      Display103_Location();
    }
    else        
    {   
      if (!bInSilent)
      {              
        Display_Voice();
      }
    }
  }
}

void DecreaseVoice(void)
{  
  /*The RED LED shitf when Press BUTTON3*/
  if (IsButtonPressed(BUTTON1))
  {
    LedOn(RED);
  }
  else
  {
    LedOff(RED);
  }
  
  /* Press BUTTON1 to decrease the voice */
  if (WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_16_BLADE_CS;  // clear /CS bit
    AT91C_BASE_PIOA->PIO_CODR |= PA_14_BLADE_MOSI;  // clear U/D bit to decrease
    
    /* realize an up-to-down signal */
    for (u16 i=0; i<10; i++)
    {
      AT91C_BASE_PIOA->PIO_SODR |= PA_12_BLADE_UPOMI; // set /INC bit 
      for (u16 j=0; j<1000; j++);
      AT91C_BASE_PIOA->PIO_CODR |= PA_12_BLADE_UPOMI; // clear /INC bit 
    }
    
    s8VoiceIndex -= 10;
    if (s8VoiceIndex < 10)
    {
      s8VoiceIndex = 0;
    }
    
    if (bInADCMode)        //Mode: Display X9C103 Location
    {
      Display103_Location();
    }
    else
    { 
      if (!bInSilent)     //Mode: Mobile and MICI 
      {
        Display_Voice();
      }
    }
  }
 
}
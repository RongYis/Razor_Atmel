/* This file is for user to modify the voice */

#include "configuration.h"

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
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;  // clear /CS bit
    AT91C_BASE_PIOA->PIO_SODR |= PA_14_BLADE_MOSI;  // set U/D bit to increase
    
    /* realize an up-to-down signal */
    for (u8 i=0; i<100; i++)
    {
      AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO; // set /INC bit
    }
    for (u8 i=0; i<100; i++)
    {
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //clear /INC bit
    }
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;  // set /CS bit
    AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO; // set /INC bit   
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
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;  // clear /CS bit
    AT91C_BASE_PIOA->PIO_CODR |= PA_14_BLADE_MOSI;  // set U/D bit to decrease
    
    /* realize an up-to-down signal */
    for (u8 i=0; i<100; i++)
    {
      AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO; // set /INC bit
    }
    for (u8 i=0; i<100; i++)
    {
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //clear /INC bit
    }
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;  // set /CS bit
    AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO; // set /INC bit   
  }
}
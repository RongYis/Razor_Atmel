/* This file is for choosing channel 
* status1: mobile input 
  status2: MICI input
  status3: no voice
*/

#include "configuration.h"

void ChangeChannel(void)
{
  static u8 u8ChooseChannelIndex = 0;
  
  /*The RED LED shitf when Press BUTTON3*/
  if (IsButtonPressed(BUTTON3))
  {
    LedOn(RED);
  }
  else
  {
    LedOff(RED);
  }
  
  /*Press BUTTON3 to choose channel*/
  if (WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    
    u8ChooseChannelIndex++;
    if (u8ChooseChannelIndex == 3)
    {
      u8ChooseChannelIndex = 0;
    }
    
    ChooseChannel(u8ChooseChannelIndex);//choose the current channel
  }
}

/*Function to Choose Current Channel*/
static void ChooseChannel(u8 u8ChannelNumber)
{
  switch (u8ChannelNumber)
  {
    case 0:
      LedOff(BLUE);
      LedOff(PURPLE);
      LedOn(GREEN);
      
      /* Enable the MObile Input:an0:0 an1:0 imo:0 */
      AT91C_BASE_PIOB->PIO_CODR |= PB_03_BLADE_AN0;   //A
      AT91C_BASE_PIOB->PIO_CODR |= PB_04_BLADE_AN1;   //B
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //C
      
      break;
    case 1:
      LedOff(PURPLE);
      LedOff(GREEN);
      LedOn(BLUE);
      
      /* Enable the MICI Input:an0:1 an1:0 imo:0 */
      AT91C_BASE_PIOB->PIO_SODR |= PB_03_BLADE_AN0;   //A
      AT91C_BASE_PIOB->PIO_CODR |= PB_04_BLADE_AN1;   //B
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //C
      
      break;
    case 2:
      LedOff(BLUE);
      LedOff(GREEN);
      LedOn(PURPLE);
      
      /* Enable the no-voice Input:an0:1 an1:1 imo:0 */
      AT91C_BASE_PIOB->PIO_SODR |= PB_03_BLADE_AN0;   //A
      AT91C_BASE_PIOB->PIO_SODR |= PB_04_BLADE_AN1;   //B
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //C
      
      break;
    default:
      break;
  }
}
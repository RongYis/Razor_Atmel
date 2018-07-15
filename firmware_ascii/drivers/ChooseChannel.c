/* This file is for choosing channel 
* status1: mobile input 
  status2: MICI input
  status3: no voice
*/

#include "configuration.h"

bool bInADCMode = FALSE;
bool bInSilent  = FALSE;

void ChangeChannel(void)
{
  static u8 u8ChooseChannelIndex = 0;
  
  /*The RED LED shitf when Press BUTTON3*/
  if (IsButtonPressed(BUTTON3) || IsButtonPressed(BUTTON2))
  {
    LedOn(RED);
  }
  else
  {
    LedOff(RED);
  }
  
  /*Press BUTTON3 to choose channel mode*/
  if (WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    
    AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;  //clear   RE  bit
    
    LedOff(WHITE);
    
    bInADCMode = FALSE;
    
    u8ChooseChannelIndex++;
    if (u8ChooseChannelIndex > NO_VOICE_STATUS)
    {
      u8ChooseChannelIndex = 0;
    }
    
    LCDCommand(LCD_CLEAR_CMD);
    ChooseChannel(u8ChooseChannelIndex);//choose the current channel
  }
  
  /*Press BUTTON2 to display the X9C103 Location*/
  if (WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    
    LedOn(WHITE);
    LedOff(GREEN);
    LedOff(PURPLE);
    LedOff(BLUE);
    
    LCDCommand(LCD_CLEAR_CMD);
    
    bInADCMode = TRUE;
    
    /* Enable the ADC Input:SCK:1 an1:1 imo:1 */
    AT91C_BASE_PIOA->PIO_CODR |= PA_15_BLADE_SCK;   //A
    AT91C_BASE_PIOB->PIO_CODR |= PB_04_BLADE_AN1;   //B
    AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO; //C
    
    AT91C_BASE_PIOA->PIO_SODR |= PA_13_BLADE_MISO;  //set RE bit
    
    Display103_Location();
    
  }
  
}

/*Function to Choose Current Channel*/
static void ChooseChannel(u8 u8ChannelNumber)
{
  switch (u8ChannelNumber)
  {
    case MOBILE_STATUS:
      LedOff(BLUE);
      LedOff(PURPLE);
      LedOn(GREEN);
      bInSilent = FALSE;
      
      /* Enable the MObile Input:SCK:0 an1:0 imo:0 */
      AT91C_BASE_PIOA->PIO_CODR |= PA_15_BLADE_SCK;   //A
      AT91C_BASE_PIOB->PIO_CODR |= PB_04_BLADE_AN1;   //B
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //C
      
      Display_Voice();
      LCDMessage(LINE2_START_ADDR, "Channel : Mobile  ");
      
      break;
      
    case MICI_STATUS:
      LedOff(PURPLE);
      LedOff(GREEN);
      LedOn(BLUE);
      bInSilent = FALSE;
      
      /* Enable the MICI Input:SCK:1 an1:0 imo:0 */
      AT91C_BASE_PIOA->PIO_SODR |= PA_15_BLADE_SCK;   //A
      AT91C_BASE_PIOB->PIO_CODR |= PB_04_BLADE_AN1;   //B
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //C
      
      Display_Voice();
      LCDMessage(LINE2_START_ADDR, "Channel : MICI   ");
      
      break;
      
    case NO_VOICE_STATUS:
      LedOff(BLUE);
      LedOff(GREEN);
      LedOn(PURPLE);
      
      bInSilent = TRUE;
      
      /* Enable the no-voice Input:SCK0:1 an1:1 imo:0 */
      AT91C_BASE_PIOA->PIO_SODR |= PA_15_BLADE_SCK;   //A
      AT91C_BASE_PIOB->PIO_SODR |= PB_04_BLADE_AN1;   //B
      AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO; //C
      
      LCDMessage(LINE1_START_ADDR, "Voice   : 000 %      ");
      LCDMessage(LINE2_START_ADDR, "Channel : No-Voice   ");
      
      break;
      
    default:
      break;
  }
  
}
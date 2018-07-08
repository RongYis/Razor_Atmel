/* This file is for choosing channel 
* status1: mobile input 
  status2: MICI input
  status3: no voice
*/

#include "configuration.h"

/*Function to Choose Current Channel*/
void ChooseChannel(u8 u8ChannelNumber)
{
  switch (u8ChannelNumber)
  {
    case 0:
      LedOn(GREEN);
      AT91C_BASE_PIOA->PIO_CODR |= MOBILE_INPUT_GPIO;//Enable the MObile Input
      break;
    case 1:
      LedOn(BLUE);
      AT91C_BASE_PIOA->PIO_SODR |= MICI_INPUT_GPIO;//Enable the MICI Input
      break;
    case 2:
      LedOn(PURPLE);
      AT91C_BASE_PIOA->PIO_SODR |= NO_VOICE_GPIO;//Enable the No-Voice
      break;
  }
}
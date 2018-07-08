/* This file is for GPIO configuration */

/* header file */
#include "configuration.h"

/* function for configuration */
void GpioConfigue(void)
{
  AT91C_BASE_PIOA->PIO_PER = GPIOA_ENABLE_CONFIG;
  AT91C_BASE_PIOA->PIO_OER = GPIOA_OUTPUT_CONFIG;
  AT91C_BASE_PIOA->PIO_SODR = GPIOA_HIGHDATA_CONFIG;
  AT91C_BASE_PIOA->PIO_CODR = GPIOA_LOWDATA_CONFIG;
}


/**********************************************************************************************************************
File: GpioConfig.h                                                                

Description:
Header file for GpioConfig.c

**********************************************************************************************************************/

#ifndef _GPIO_CONFIG_H_
#define _GPIO_CONFIG_H_

/*const definition*/
#define GPIOA_ENABLE_CONFIG (u32)0x0001FE00
  /*16 [1] PA_16_BLADE_CS PIO control enabled

    15 [0] PA_15_BLADE_SCK PIO control not enabled
    14 [0] PA_14_BLADE_MOSI PIO control not enabled
    13 [0] PA_13_BLADE_MISO PIO control not enabled
    12 [0] PA_12_BLADE_UPOMI PIO control not enabled

    11 [0] PA_11_BLADE_UPIMO PIO control not enabled
    10 [0] PA_10_I2C_SCL PIO control not enabled
    09 [0] PA_09_I2C_SDA PIO control not enabled*/

#define GPIOA_OUTPUT_CONFIG (u32)0x0001FE00
   /*16 [1] PA_16_BLADE_CS output enabled

    15 [1] PA_15_BLADE_SCK output enabled
    14 [1] PA_14_BLADE_MOSI output enabled
    13 [0] PA_13_BLADE_MISO output enabled
    12 [1] PA_12_BLADE_UPOMI output enabled

    11 [0] PA_11_BLADE_UPIMO  output enabled
    10 [1] PA_10_I2C_SCL output enabled
    09 [1] PA_09_I2C_SDA output enabled*/

#define GPIOA_HIGHDATA_CONFIG (u32)0x00010000
   /*16 [1] PA_16_BLADE_CS output high*/

#define GPIOA_LOWDATA_CONFIG (u32)0x0000FE00
   /*15 [0] PA_15_BLADE_SCK output low
     14 [0] PA_14_BLADE_MOSI output low
     13 [0] PA_13_BLADE_MISO output low
     12 [0] PA_12_BLADE_UPOMI output low

     11 [0] PA_11_BLADE_UPIMO output low
     10 [0] PA_10_I2C_SCL output low
     09 [0] PA_09_I2C_SDA output low*/



void GpioConfigue(void);

#endif


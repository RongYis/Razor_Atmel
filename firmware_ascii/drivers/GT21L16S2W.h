/* This File is the GT21L16S2W.c head file */

#ifndef _GT21L16S2W_H_
#define _GT21L16S2W_H_

/* constant value */
#define ADDRESS_NUMBERS   (u8)3
#define GET_WORD_CMD      (u8)0x03
#define ONE_WORD_BYTES    (u8)32
#define BASEADDRESS       (u32)0

bool GetWord_From_Debug(void);

void GetData_From_GT21L16S2W(u16* u16Word);

static u32 Get_Word_Address(u16 u16OneWord);

static void Send_SI_BYTE(u8 u8SendData);

static void Send_IN_EACH_BIT(u8 u8SendData);

static u8* Get_One_Word(u16 u16OneWord);

static u8 Get_Each_Byte(void);

static u8 Get_Each_Bit(void);

#endif
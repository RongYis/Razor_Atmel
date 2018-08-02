/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define COUNTER_LIMIT_MS   (u32)10
#define LINE_MAX_NUMBER    (u8)16
#define ROW_MAX_NUMBER     (u8)10
#define ONE_BYTE_TO_BITS   (u8)8
#define BYTE_MAX_BIT       (u8)0x80
#define BYTE_MIN_BIT       (u8)0x01
#define FIRST_BIT          (u8)0x1
#define SECOND_BIT         (u8)0x2
#define THIRD_BIT          (u8)0x4
#define FORTH_BIT          (u8)0x8
#define ROLL_TIME          (u8)150

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         

static void Delay_1s(void);

static void clear_array(u8 u8SelectArray[][100]);

static void Screen_Display(void);

static void Display_One_Line(u8 u8LineNumber);

static void Clear_One_Line(void);

static void Screen_Roll_Left(void);

static void Row_Choose_Control(u8 u8Data);

static void Line_Choose_Control(u8 u8LineNumber);

static void GetChineseData(u8 au8GetChinese[][LINE_MAX_NUMBER*2]);

static void Send_One_Line_Data(u8* au8DataArray);
#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

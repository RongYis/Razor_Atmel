/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */

extern u8 u8WordNumber;
extern  u8 au8ChineseWord[][LINE_MAX_NUMBER*2];
extern u8 au8GetDataFromFM24CL16B[50][LINE_MAX_NUMBER*2];



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static u8 au8ChineseArray[LINE_MAX_NUMBER][100];  /* Save the CHinese Word */
bool bWriteIsOn = FALSE;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  read_out_data();    //从存储芯片中读取数据
  GetChineseData(au8GetDataFromFM24CL16B);
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u16 u16TimeCounter = 0; 
  
  if (GetWord_From_Debug())
  {
    clear_array(au8ChineseArray);    //清除数组元素
    bWriteIsOn = TRUE;
    read_out_data();
    bWriteIsOn = FALSE;
    GetChineseData(au8GetDataFromFM24CL16B);
  }
  
  u16TimeCounter++;   //滚动时间计数器
  
  Screen_Display();
  
  if (u16TimeCounter == ROLL_TIME)  //达到滚屏时间
  {
    u16TimeCounter = 0;
    Screen_Roll_Left();  //向左滚屏一列
  }
}


/* 此函数用来清空数组 */
static void clear_array(u8 u8SelectArray[][100])
{
  for (u8 i=0; i<LINE_MAX_NUMBER; i++)
  {
    for (u8 j=0; j<100; j++)
    {
      u8SelectArray[i][j] = 0;
    }
  }
}

/* 每切换一行显示一行 */
static void Screen_Display(void)
{
  static u8 u8LineNumber = 0;
  
  Clear_One_Line();   //清除一行的数据
  
  /* 选择 行数 */
  Line_Choose_Control(u8LineNumber);
  
  Display_One_Line(u8LineNumber);  //显示这一行的数据
  
  u8LineNumber++;
  if (u8LineNumber == LINE_MAX_NUMBER)
  {
    u8LineNumber = 0;  //返回第一行
  } 
   
}

/* 发送一行的数据 */
static void Send_One_Line_Data(u8* au8DataArray)
{
  static u8 u8Data = 0;
  
  AT91C_BASE_PIOA->PIO_SODR  =  PA_12_BLADE_UPOMI;  //M_LE-----1
  AT91C_BASE_PIOA->PIO_SODR  =  PA_11_BLADE_UPIMO;  //M_OE-----1
  
  for (u8 j=0; j<ROW_MAX_NUMBER; j++)  //循环10个字节
  {
    u8Data = *(au8DataArray+ROW_MAX_NUMBER-1-j);  // 获取其中一个字节数据
    
    for (u8 i=0; i<ONE_BYTE_TO_BITS; i++)   //每个字节循环8位
    {      
      Row_Choose_Control(u8Data);   //SDI输入数据
      u8Data = u8Data>>1;    //数据左移一位
      
      AT91C_BASE_PIOA->PIO_CODR  =  PA_15_BLADE_SCK;  //M_CLK-----0  
      AT91C_BASE_PIOA->PIO_SODR  =  PA_15_BLADE_SCK;  //M_CLK-----1  
    }
  }
}


/* 显示一行的数据 */
static void Display_One_Line(u8 u8LineNumber)
{
  Send_One_Line_Data(*(au8ChineseArray+u8LineNumber));  //发送汉字数据
  
  AT91C_BASE_PIOA->PIO_CODR  =  PA_11_BLADE_UPIMO;  //M_OE-----0 开始输出  
}


/* 清除一行的显示 */
static void Clear_One_Line(void)
{
  static u8 au8ClearArray[ROW_MAX_NUMBER];
  
  /* 初始化一行清除数据 */
  for (u8 i=0; i<ROW_MAX_NUMBER; i++)
  {
     *(au8ClearArray+i) = 0x00;
  }
  
  Send_One_Line_Data(au8ClearArray);   //发送清除数据
}


/* 获取中文字模数据放进 au8ChineseArray 数组中 */
static void GetChineseData(u8 au8GetChinese[][LINE_MAX_NUMBER*2])
{
  static u8 u8ChineseLineIndex = 0;
  
  /* 将中文数据 au8ChineseWord 的每一行的两列 放到
  au8ChineseArray 的两列中*/
  for (u8 j=0; j<(u8WordNumber/2); j++)
  {
    for (u8 i=0; i<LINE_MAX_NUMBER; i++)
    {   
      au8ChineseArray[i][2*j-2*u8ChineseLineIndex] = au8GetChinese[j][2*i];
      au8ChineseArray[i][2*j+1-2*u8ChineseLineIndex] = au8GetChinese[j][2*i+1];
    }
  }  
  
}


/* 屏幕开始向左滚动一列 */
static void Screen_Roll_Left(void)
{
  static u8 u8FirstRowData = 0;
  
  for (u8 i=0; i<LINE_MAX_NUMBER; i++)
  {
    /* 获取每一列第一个数据 */
    u8FirstRowData = au8ChineseArray[i][0] & BYTE_MAX_BIT;
    for (u8 j=0; j<(u8WordNumber/2+5)*2; j++)
    {
      /* 处理前 Word_NUMBER-1 个数据 */
      if (j < (u8WordNumber/2+5)*2-1)
      {
        au8ChineseArray[i][j] = au8ChineseArray[i][j]<<1;   //数据左移
        
        /* 将下一个字节的第一位放在当前字节的最后一位 */
        if ((au8ChineseArray[i][j+1] &BYTE_MAX_BIT) == BYTE_MAX_BIT)
        {
          au8ChineseArray[i][j] = (au8ChineseArray[i][j]&0xFE)|BYTE_MIN_BIT;
        }
      }
      else  /*处理第 Word_NUMBER 个数据 */
      {
        au8ChineseArray[i][j] = au8ChineseArray[i][j]<<1;  //数据左移
        
        /* 将每一列获得的第一位放到最后一列，即放到最后一个字节的最后一位 */
        if ((u8FirstRowData &BYTE_MAX_BIT) == BYTE_MAX_BIT)
        {
          au8ChineseArray[i][j] = (au8ChineseArray[i][j]&0xFE)|BYTE_MIN_BIT;
        }
      }
    }
  }
}


/* 检查每个字节的最高位来控制SDI的高低电平 */
static void Row_Choose_Control(u8 u8Data)
{
  if ((u8Data & BYTE_MIN_BIT) == BYTE_MIN_BIT)
  {
    AT91C_BASE_PIOA->PIO_SODR  =  PA_14_BLADE_MOSI; //M_SDI-----1
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR  =  PA_14_BLADE_MOSI; //M_SDI-----0
  }
}


/* 利用行号来控制 A B C D的高低电平 */
static void Line_Choose_Control(u8 u8LineNumber)
{
  AT91C_BASE_PIOA->PIO_SODR  =  PA_03_HSMCI_MCCK;   //STB---1
  AT91C_BASE_PIOA->PIO_CODR  =  PA_04_HSMCI_MCCDA;  //INH---0
  
  if ((u8LineNumber & FIRST_BIT) == FIRST_BIT)
  {
    AT91C_BASE_PIOA->PIO_SODR  =  PA_05_HSMCI_MCDA0;  //A-----1
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR  =  PA_05_HSMCI_MCDA0;  //A-----0
  }
  
  if ((u8LineNumber & SECOND_BIT) == SECOND_BIT)
  {
    AT91C_BASE_PIOA->PIO_SODR  =  PA_06_HSMCI_MCDA1;  //B-----1
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR  =  PA_06_HSMCI_MCDA1;  //B-----0
  }
  
  if ((u8LineNumber & THIRD_BIT) == THIRD_BIT)
  {
    AT91C_BASE_PIOA->PIO_SODR  =  PA_07_HSMCI_MCDA2;  //C-----1
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR  =  PA_07_HSMCI_MCDA2;  //C-----0
  }
  
  if ((u8LineNumber & FORTH_BIT) == FORTH_BIT)
  {
    AT91C_BASE_PIOA->PIO_SODR  =  PA_08_SD_CS_MCDA3;  //D-----1
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR  =  PA_08_SD_CS_MCDA3;  //D-----0
  }
}


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"
#include "OLED.h"
#include "Buzzer.h"

u8 Flag_Key_Right=0;
u8 Flag_Key_Left=0;
u8 Flag_Key_Mid=0;
u8 Flag_Key_Menu=0;
extern u8 mr;
extern u8 Clear;
extern u8 Enter_Level;
extern u8 Enter_Regular;
extern u8 Enter_RGB;


//********************************************************初始化
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB ,&GPIO_InitStructure);
}

//********************************************************左
u8 Key_Left(void)  
{
	Buzzer_Init ();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)
	{
		Delay_ms (20);
		
		Buzzer_ON ();   //操作提示音
		Delay_ms (20);
		Buzzer_OFF ();
		
		Flag_Key_Left=1;
		
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0);    //如果按键一直按下，就卡在这
		Delay_ms (20);
	}
	return Flag_Key_Left;
}

//**********************************************************右
u8 Key_Right(void)  
{
	Buzzer_Init ();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
	{
		Delay_ms (20);

		Buzzer_ON ();   //操作提示音
		Delay_ms (20);
		Buzzer_OFF ();
		
		Flag_Key_Right=1;
		
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0);    //如果按键一直按下，就卡在这
		Delay_ms (20);
	}
	return Flag_Key_Right;
}
 
//**********************************************************中
u8 Key_Mid(void) 
{
	Buzzer_Init ();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		Delay_ms (20);
		
		Enter_Level ++;
		
		if(Enter_Level ==4)
		{
			Enter_Level=2;
		}
		
		if(Enter_Level ==2)
		{
			Enter_Regular=1;
			Enter_RGB=1;
		}
		
		if(Enter_Level ==3)
		{
			
		}
		
		Clear=1;
		
		Buzzer_ON ();   //操作提示音
		Delay_ms (20);
		Buzzer_OFF ();
		
		Flag_Key_Mid=1;
		
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);    //如果按键一直按下，就卡在这
		Delay_ms (20);
	}
	return Flag_Key_Mid;
}

//***********************************************************设置
uint8_t Key_Menu(void)  
{
	Buzzer_Init ();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		Delay_ms (20);
		
		mr=0;   //退出默认状态
		Clear=1;
		Enter_Level=0;
		
		Buzzer_ON ();   //操作提示音
		Delay_ms (20);
		Buzzer_OFF ();
		
		Flag_Key_Menu=1;
		
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);    //如果按键一直按下，就卡在这
		Delay_ms (20);
	}
	return Flag_Key_Menu;
}

//**********************************************************按键扫描
void Key_Scan(void)
{
	Key_Mid();  
	Key_Right();  
	Key_Left();  
	Key_Menu();  
}	

//**********************************************************标志位清0
void Key_Clear(void)
{
	Flag_Key_Right=0;
	Flag_Key_Left=0;
	Flag_Key_Mid=0;
	Flag_Key_Menu=0;
}	

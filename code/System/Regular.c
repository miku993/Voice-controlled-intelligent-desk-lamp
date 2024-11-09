#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "OLED.h"
#include "Key.h"

extern u8 Flag_Key_Left;
extern u8 Flag_Key_Mid;
extern u8 Flag_Key_Right;
extern u8 Clear;
u8 Regular_Mode_Num=1;
extern u8 Enter_Regular;

//*******************************************************************模式判断
void Regular_Mode(void)
{
	if(Flag_Key_Right==1)
	{
		Regular_Mode_Num++;
	}
	
	else if(Regular_Mode_Num==3)
	{
		Regular_Mode_Num=1;
	}
	
	else if(Regular_Mode_Num==0)
	{
		Regular_Mode_Num=2;
	}
}

//*******************************************************************
void Regular(void)  
{
	Regular_Mode();
	
	if(Regular_Mode_Num==1)  //定时开启
	{
		OLED_Clear_One();
		Key_Scan();
		if(Flag_Key_Mid ==1)
		{
			
		}
		else
		    OLED_Show_Regular_Open();
	}
	
	if(Regular_Mode_Num==2)  //定时关闭
	{
		OLED_Clear_One();
		OLED_Show_Regular_Close();
	}
}



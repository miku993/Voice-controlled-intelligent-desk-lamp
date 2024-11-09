#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "OLED.h"
#include "Dimming.h"
#include "RGB.h"
#include "Regular.h"


extern u8 Flag_Key_Right;
extern u8 Flag_Key_Left;
extern u8 Flag_Key_Mid;
extern u8 Flag_Key_Menu;
u8 Menu_Mode_Num=1;
extern u8 Clear;
extern u8 Dimming_Mode_Num;
extern u8 Enter_Level;  //进入子程序标志位
u8 Enter_Regular=0;
u8 Enter_RGB=0;

//***********************************************************模式判断
void Menu_Mode(void)
{
	if(Flag_Key_Left==1)
	{
		Clear=1;
		Menu_Mode_Num--;
	}
		
	else if(Flag_Key_Right==1)
	{
		Clear=1;
		Menu_Mode_Num++;
	}
		
	else if(Menu_Mode_Num==4)
	{
		Menu_Mode_Num=1;
	}
	
	else if(Menu_Mode_Num==0)
	{
		Menu_Mode_Num=3;
	}
}

//*******************************************************模式进入
void Menu_Enter(void)
{
	if(Menu_Mode_Num==1)  //办公模式
	{
		Dimming();
	}
	
	else if(Menu_Mode_Num==2)  //氛围模式
	{
		if(Enter_Level==1 || Enter_Level==3)
			RGB();
		if(Enter_Level==2)
			Dimming_RGB();
	}
	
	else if(Menu_Mode_Num==3)  //定时模式
	{
		Regular();
	}
}

//*******************************************************设置界面
void Menu(void)
{
	Menu_Mode();
	
	if(Menu_Mode_Num==1)  //办公模式
    {
	    OLED_Clear_One();
    	OLED_Show_Menu_LED();
    }

    else if(Menu_Mode_Num==2)  //氛围模式
    {
	    OLED_Clear_One();
	    OLED_Show_Menu_RGB();
		
    }
	 
    else if(Menu_Mode_Num==3)  //定时模式
    {
	    OLED_Clear_One();
	    OLED_Show_Menu_Regular();
    }
}

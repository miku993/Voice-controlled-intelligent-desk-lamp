#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Key.h"
#include "OLED.h"
#include "RGB.h"

extern u8 Flag_Key_Left;
extern u8 Flag_Key_Right;
extern u8 Clear;
u8 Dimming_Mode_Num=3;
extern u8 i;

//*******************************************************************模式判断
void Dimming_Mode(void)
{
	if(Flag_Key_Right==1)
	{
		Dimming_Mode_Num--;
	}
		
	else if(Flag_Key_Left==1)
	{
		Dimming_Mode_Num++;
	}
	
	else if(Dimming_Mode_Num==6)
	{
		Dimming_Mode_Num=1;
	}
	
	else if(Dimming_Mode_Num==0)
	{
		Dimming_Mode_Num=5;
	}
}

//*******************************************************************LED调光
void Dimming(void)  
{
	Dimming_Mode();
	
	if(Dimming_Mode_Num==1)  //0%
	{
		OLED_Clear_One();
		PWM_SetCompare1(0);
		
		OLED_ShowString (3,6," ");  //修边
		OLED_ShowString (3,9," ");
		OLED_Show_LED();
		OLED_ShowString (3,7,"0%");
	}
	
	else if(Dimming_Mode_Num==2)  //25%
	{
		OLED_Clear_One();
		PWM_SetCompare1(25);
		
		OLED_Show_LED();
		OLED_ShowString (3,7,"25%");
	}
	
	else if(Dimming_Mode_Num==3)  //50% -- 默认
	{
		OLED_Clear_One();
		PWM_SetCompare1(50);
		
		OLED_Show_LED();
		OLED_ShowString (3,7,"50%");
	} 
	
	else if(Dimming_Mode_Num==4)  //75%
	{
		OLED_Clear_One();
		PWM_SetCompare1(75);
		
		OLED_ShowString (3,6," ");  //修边
		OLED_Show_LED();
		OLED_ShowString (3,7,"75%");
	}
	
	else if(Dimming_Mode_Num==5)  //100%
	{
		OLED_Clear_One();
		PWM_SetCompare1(100);
		
		OLED_Show_LED();	
		OLED_ShowString (3,6,"100%");
	}
}

//***********************************************************RGB调光
void Dimming_RGB(void)
{
	u8 Color;
	Color=RGB_Mode_Number();
	Dimming_Mode();
	
	if(Dimming_Mode_Num==1)  //0%
	{
		OLED_Clear_One();
		i=0;
		RGB_Color(Color,i);
		
		OLED_ShowString (3,6," ");  //修边
		OLED_ShowString (3,9," ");
		OLED_Show_RGB_Dimming();
		OLED_ShowString (3,7,"0%");
	}
	
	else if(Dimming_Mode_Num==2)  //25%
	{
		OLED_Clear_One();
		i=25;
		RGB_Color(Color,i);
		
		OLED_Show_RGB_Dimming();
		OLED_ShowString (3,7,"25%");
	}
	
	else if(Dimming_Mode_Num==3)  //50% -- 默认
	{
		OLED_Clear_One();
		i=50;
		RGB_Color(Color,i);
		
		OLED_Show_RGB_Dimming();
		OLED_ShowString (3,7,"50%");
	}
	
	else if(Dimming_Mode_Num==4)  //75%
	{
		OLED_Clear_One();
		i=75;
		RGB_Color(Color,i);
		
		OLED_ShowString (3,6," ");  //修边
		OLED_Show_RGB_Dimming();
		OLED_ShowString (3,7,"75%");
	}
	
	else if(Dimming_Mode_Num==5)  //100%
	{
		OLED_Clear_One();
		i=100;
		RGB_Color(Color,i);
		
		OLED_Show_RGB_Dimming();
		OLED_ShowString (3,6,"100%");
	}
}

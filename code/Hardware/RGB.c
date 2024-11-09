#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Key.h"
#include "OLED.h"
#include "Dimming.h"
#include "Delay.h"
#include "Timer.h"

#define Gradient 1
#define Red 2
#define Green 3
#define Blue 4
#define Yellow 5
#define Purple 6
#define Cyan 7
#define Black 8

u8 RGB_Mode_Num=1;
u8 i=50;
u8 New=1;
u8 Gradient_State=0;  //渐变色状态
u8 Temp;

extern u8 Flag_Key_Left;
extern u8 Flag_Key_Right;
extern u8 Flag_Key_Mid; 
extern u8 Clear;
extern u8 Time_RGB;

//*******************************************************************初始化
void RGB_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE );
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);
	
	GPIO_SetBits (GPIOA ,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}

//*******************************************************************颜色选择
u8 RGB_Color(u8 color,u8 i)
{
	switch(color)
    {
       	case Gradient:  //1
		{
			TIM3_Open();																																																																																																																																																																																										
			if(i==0)
			{
				PWM_SetCompare2(0);
                PWM_SetCompare3(0);	
                PWM_SetCompare4(0);	
			}
			
			else
			{
				if (Gradient_State == 0)  //Blue up  --Blue
			    {		
					if (New == 1)
				    {
					    Temp = 0;
					    New = 0;
				    }
				        Temp ++;
				        PWM_SetCompare4(Temp);	
				        while (Time_RGB == 0);
				        if (Temp >= i)
				        {
					        Gradient_State = 1;
					        New = 1;
           				}
           					
           			}
           			else if (Gradient_State == 1) //Green up  --Cyan
           			{
           				if (New == 1)
           				{
           					Temp = 0;
           					New = 0;
           				}
           				Temp ++;
           			    PWM_SetCompare3(Temp);
           				while (Time_RGB == 0);
           				if (Temp >= i)
           				{
           					Gradient_State = 2;
           					New = 1;
           				}
           			}
           			else if (Gradient_State == 2)  //Blue down  --Green
           			{
           			    if (New == 1)
           				{
           					Temp = 0;
           					New = 0;
           				}
           				Temp ++;
           				PWM_SetCompare4(i - Temp);
           				while(Time_RGB == 0);
           				if(Temp >= i)
           				{
           					Gradient_State = 3;
           					New = 1;
           				}
           			}
           			else if (Gradient_State == 3)  //Red up  --Red and Blue
           			{
           			    if (New == 1)
           				{
           					Temp = 0;
           					New = 0;
           				}
           				Temp ++;
           				PWM_SetCompare2(Temp);
           				while (Time_RGB == 0);
           				if (Temp >= i)
           				{
           					Gradient_State = 4;
           					New = 1;
           				}
           			}
           			else if (Gradient_State == 4)  //Green down  --Red
           			{
           			    if (New == 1)
           				{
           					Temp = 0;
           					New = 0;
           				}
           				Temp ++;
           				PWM_SetCompare3(i - Temp); 
           				while (Time_RGB == 0);
           				if (Temp >= i)
           				{
           					Gradient_State = 5;
           					New = 1;
           				}
           			}
           			else if (Gradient_State == 5)  //Blue up  --Purple
           			{
           			    if (New == 1)
           				{
           					Temp = 0;
           					New = 0;
           				}
           				Temp ++;
           				PWM_SetCompare4(Temp); 	
                           while (Time_RGB == 0);
           				if (Temp >= i)
           				{
           					Gradient_State = 6;
           					New=1;
           				}
           			}
           			else if (Gradient_State == 6)  //Blue down,Red down  --Dark
           			{
           				if (New == 1)
           				{
           					Temp = 0;
           					New = 0;
           				}
           				Temp ++;
           				PWM_SetCompare4(i - Temp);  
           				PWM_SetCompare2(i - Temp);
           				while (Time_RGB == 0);
           				if (Temp >= i)
           				{
           					Gradient_State = 0;
           					New = 1;
           				}
           			}
        		}
		
        }break;
			
		case Red:  //2
		{
			TIM3_Close();
			PWM_SetCompare2(i);
			PWM_SetCompare3(0);
            PWM_SetCompare4(0);
        }break;
		
        case Green:  //3
		{
			PWM_SetCompare2(0);
            PWM_SetCompare4(0);
            PWM_SetCompare3(i);
        }break;
		
        case Blue:  //4
		{
			PWM_SetCompare3(0);
            PWM_SetCompare2(0);
            PWM_SetCompare4(i);
        }break;
		
        case Yellow:  //5
		{
			PWM_SetCompare4(0);
            PWM_SetCompare3(i);
            PWM_SetCompare2(i);
        }break;
		
        case Purple:  //6
		{
			PWM_SetCompare4(i);
            PWM_SetCompare3(0);
            PWM_SetCompare2(i);
        }break;
		
        case Cyan:  //7
		{
			PWM_SetCompare4(i);
            PWM_SetCompare3(i);
            PWM_SetCompare2(0);
        }break;
		
		case Black:  //8
		{
			TIM3_Close();
			PWM_SetCompare4(0);
            PWM_SetCompare3(0);
            PWM_SetCompare2(0);
        }break;
    }
	
	return i;
}


//*******************************************************************模式判断
void RGB_Mode(void)
{
	if(Flag_Key_Right==1)
	{
		RGB_Mode_Num++;
	}
		
	else if(Flag_Key_Left==1)
	{
		RGB_Mode_Num--;
	}
	
	else if(RGB_Mode_Num==9)
	{
		RGB_Mode_Num=1;
	}
	
	else if(RGB_Mode_Num==0)
	{
		RGB_Mode_Num=8;
	}
}

//*******************************************************************模式切换
void RGB(void)
{
	RGB_Mode();
	
	if(RGB_Mode_Num==1)  //渐变
	{
		OLED_Clear_One();
		RGB_Color(1,i);
		OLED_Show_RGB_Gradient();
	}
		
	else if(RGB_Mode_Num==2)  //红
	{
        OLED_ShowCN(2,2,30);  //修边
		OLED_ShowCN(2,5,30);
		OLED_Clear_One();
		RGB_Color(2,i);
        OLED_Show_RGB_Red();		
	}
		
	else if(RGB_Mode_Num==3)  //绿
	{
		OLED_Clear_One();
		RGB_Color(3,i);
		OLED_Show_RGB_Green();
	}
	
	else if(RGB_Mode_Num==4)  //蓝
	{
		OLED_Clear_One();
		RGB_Color(4,i);
		OLED_Show_RGB_Blue();
	}
	
	else if(RGB_Mode_Num==5)  //黄
	{
		OLED_Clear_One();
		RGB_Color(5,i);
		OLED_Show_RGB_Yellow();
	}
	
	else if(RGB_Mode_Num==6)  //紫
	{
		OLED_Clear_One();
		RGB_Color(6,i);
		OLED_Show_RGB_Purple();
	}
	
	else if(RGB_Mode_Num==7)  //青
	{		
		OLED_Clear_One();
		RGB_Color(7,i);
		OLED_Show_RGB_Cyan();
	}
	
	else if(RGB_Mode_Num==8)  //关闭
    {
		OLED_ShowCN(2,2,30);  //修边
	    OLED_ShowCN(2,5,30);
		
	    OLED_Clear_One();
	    RGB_Color(8,i);
	    OLED_Show_RGB_Close();
    }
	
}

//*******************************************************************模式传递
u8 RGB_Mode_Number(void)
{
	return RGB_Mode_Num ;
}

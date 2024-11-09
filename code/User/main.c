#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Key.h"
#include "Buzzer.h"
#include "Timer.h"
#include "Dimming.h"
#include "Menu.h"
#include "RGB.h"
#include "Regular.h"
#include "LED.h"
#include "Serial.h"

extern u8 Flag_Key_Menu;
extern u8 Flag_Key_Mid;
u8 Clear=0;  //清屏标志位
u8 mr=1;  //退出默认状态标志位
u8 Enter_Level=0;  //子程序标志位
extern u8 VoiceFlag;
extern u8 Voice_Mode_Num;

int main(void)
{
	Serial_Init();
	OLED_Init();
	PWM_Init();
	Key_Init();
	Buzzer_Init();
	TIM3_Init();
//	LED_Switch_Init();
	
	//***********************************************  --默认状态
	OLED_ShowCN (1,2,0);  //办公模式
	OLED_ShowCN (1,3,1);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
		
	OLED_ShowCN (2,2,12);  //当前亮度
	OLED_ShowCN (2,3,13);
	OLED_ShowCN (2,4,14);
	OLED_ShowCN (2,5,15);
		
	OLED_ShowString (3,7,"50%");
		
	OLED_ShowCN (4,1,26);  //加
	OLED_ShowCN (4,6,27);  //减
	//***********************************************
	
	while(1)
	{
		Key_Clear ();
		Key_Scan ();
		if(VoiceFlag == 1)  //语音模式
		{
			Voice ();
			Delay_ms (1000);
			VoiceFlag=0;
			mr=0;
			Clear=1;
		}	
		else  //手动模式
		{
			
			if(mr==1)  //默认标志位
		    {
			    Dimming();
		    }
		    else
		    {
			    if(Enter_Level==0)
				    Menu ();  //主程序
			    else
				    Menu_Enter ();  //子程序
		    }
		}
		
	}
}


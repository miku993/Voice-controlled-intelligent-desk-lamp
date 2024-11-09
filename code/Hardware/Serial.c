#include "stm32f10x.h"                  // Device header
#include <string.h>  //字符串处理库
#include "Buzzer.h"
#include "OLED.h"
#include "Timer.h"
#include "Dimming.h"
#include "Menu.h"
#include "RGB.h"

u8 VoiceFlag=0;
u8 RxData;
u8 RxFlag;
char Serial_RxPacket[100];  //接收载荷数据
extern u8 Time_Voice;
extern u8 Clear;
extern u8 Dimming_Mode_Num;
extern u8 Menu_Mode_Num;
extern u8 Enter_Level;
extern u8 RGB_Mode_Num;
u8 Voice_Mode_Num;
extern u8 Menu_Mode_Num;

//***************************************************************初始化
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 ,ENABLE );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE );
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //TX用复用输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //RX用上拉或浮空输入--空闲状态为高电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init (USART1 ,&USART_InitStructure);
	
	USART_ITConfig (USART1 ,USART_IT_RXNE, ENABLE );
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init (&NVIC_InitStructure);
	
	USART_Cmd (USART1 ,ENABLE );
}

//***************************************************************中断
void USART1_IRQHandler(void )
{
	static u8 RxState=0;  //只置一次0
	static u8 i=0;
	
	if(USART_GetFlagStatus(USART1 ,USART_FLAG_RXNE) == SET)
	{
		
		RxData = USART_ReceiveData(USART1 );
		if(RxState == 0)
		{
			if(RxData == '@' && RxFlag == 0)
			{
				RxState = 1;
				i=0;
			}
		}
		else if(RxState == 1)
		{
			if (RxData == '\r')  //回车符
			{
				RxState = 2;
			}
			else
			{
				Serial_RxPacket[i] = RxData;
				i ++;
			}
		}
		else if(RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[i] = '\0';
				RxFlag = 1;  //数据接收完毕
				VoiceFlag=1;  //进入语音模式标志位
			}
		}
	}
	USART_ClearITPendingBit (USART1 ,USART_IT_RXNE );  //清除标志位
}

//************************************************************数据判断
void Serial_RxData_Judgment(void)
{
	if(RxFlag == 1)  //当串口接收完数据包
	{
		if (strcmp (Serial_RxPacket, "Welcome") == 0)
		{
			Clear =1;
			Voice_Mode_Num=0;
		}

		else if (strcmp (Serial_RxPacket, "LED_ON") == 0)
		{
			Clear =1;
			Voice_Mode_Num=1;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_OFF") == 0)
		{
			Clear =1;
			Voice_Mode_Num=2;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_Up") == 0)
		{
			Clear =1;
			Voice_Mode_Num=3;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_LOW") == 0)
		{
			Clear =1;
			Voice_Mode_Num=4;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_One") == 0)
		{
			Clear =1;
			Voice_Mode_Num=5;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_Two") == 0)
		{
			Clear =1;
			Voice_Mode_Num=6;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_Three") == 0)
		{
			Clear =1;
			Voice_Mode_Num=7;
		}
		
		else if (strcmp (Serial_RxPacket, "LED_Four") == 0)
		{
			Clear =1;
			Voice_Mode_Num=8;
		}
		
		else if (strcmp (Serial_RxPacket, "Mode_RGB_ON") == 0)
		{
			Clear =1;
			Voice_Mode_Num=9;
		}
		
		else if (strcmp (Serial_RxPacket, "Mode_RGB_OFF") == 0)
		{
			Clear =1;
			Voice_Mode_Num=10;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Red") == 0)
		{
			Clear =1;
			Voice_Mode_Num=11;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Green") == 0)
		{
			Clear =1;
			Voice_Mode_Num=12;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Blue") == 0)
		{
			Clear =1;
			Voice_Mode_Num=13;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Yellow") == 0)
		{
			Clear =1;
			Voice_Mode_Num=14;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Purple") == 0)
		{
			Clear =1;
			Voice_Mode_Num=15;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Cyan") == 0)
		{
			Clear =1;
			Voice_Mode_Num=16;
		}
		
		else if (strcmp (Serial_RxPacket, "RGB_Gradient") == 0)
		{
			Clear =1;
			Voice_Mode_Num=17;
		}
		else if (strcmp (Serial_RxPacket, "All_ON") == 0)
		{
			Clear =1;
			Voice_Mode_Num=18;
		}
		
		else if (strcmp (Serial_RxPacket, "All_OFF") == 0)
		{
			Clear =1;
			Voice_Mode_Num=19;
		}
		
		RxFlag = 0;
	}
}

//************************************************************语音模式
void Voice(void)
{
	Serial_RxData_Judgment();
	Enter_Level=1;
	
	if(Voice_Mode_Num == 0)
	{
	    OLED_Clear_One();
	    Buzzer_VoiceFeedback_1();
		OLED_Show_Voice_Yes();
	}

	else if (Voice_Mode_Num == 1)
	{
		OLED_Clear_One();
		OLED_Show_Voice_LED_ON();
		Buzzer_VoiceFeedback_2();
		
		Menu_Mode_Num=1;
		Dimming_Mode_Num=3;
	}
		
	else if (Voice_Mode_Num == 2)
	{
		OLED_Clear_One();
		OLED_Show_Voice_LED_OFF();
		Buzzer_VoiceFeedback_2();
		
		Menu_Mode_Num=1;
		Dimming_Mode_Num=1;
	}
		
	else if (Voice_Mode_Num == 3)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_LED_Up();
		
		Menu_Mode_Num=1;
		Dimming_Mode_Num++;
	}
		
	else if (Voice_Mode_Num == 4)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_LED_Low();
        
		Menu_Mode_Num=1;
		Dimming_Mode_Num--;
	}
		
	else if (Voice_Mode_Num == 5)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_LED_One();
		
		Menu_Mode_Num=1;
		Dimming_Mode_Num=2;
	}
		
	else if (Voice_Mode_Num == 6)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_LED_Two();
		
		Menu_Mode_Num=1;
		Dimming_Mode_Num=3;
	}
		
	else if (Voice_Mode_Num == 7)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_LED_Three();
		
		Menu_Mode_Num=1;
		Dimming_Mode_Num=4;
	}
		
	else if (Voice_Mode_Num == 8)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_LED_Four();
        
		Menu_Mode_Num=1;
		Dimming_Mode_Num=5;
	}
		
	else if (Voice_Mode_Num == 9)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_Mode_RGB_ON();	
			
		Menu_Mode_Num=2;
	}
		
	else if (Voice_Mode_Num == 10)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_Mode_RGB_OFF();	
		
		Menu_Mode_Num=2;
		RGB_Mode_Num=8;
	}
		
	else if (Voice_Mode_Num == 11)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_RGB_Red();	
			
		Menu_Mode_Num=2;
		RGB_Mode_Num=2;
	}
		
	else if (Voice_Mode_Num == 12)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
        OLED_Show_Voice_RGB_Green();
		
		Menu_Mode_Num=2;
		RGB_Mode_Num=3;
	}
		
	else if (Voice_Mode_Num == 13)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_RGB_Blue();	
			
		Menu_Mode_Num=2;
		RGB_Mode_Num=4;
	}
		
	else if (Voice_Mode_Num == 14)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_RGB_Yellow();	
			
		Menu_Mode_Num=2;
		RGB_Mode_Num=5;
	}
		
	else if (Voice_Mode_Num == 15)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_RGB_Purple();
		
		Menu_Mode_Num=2;
		RGB_Mode_Num=6;
	}
		
	else if (Voice_Mode_Num == 16)
	{
		OLED_Clear_One();
		Buzzer_VoiceFeedback_2();
		OLED_Show_Voice_RGB_Cyan();
			
		Menu_Mode_Num=2;
		RGB_Mode_Num=7;
	}
		
	else if (Voice_Mode_Num == 17)
	{
		OLED_Clear_One();
		OLED_Show_Voice_RGB_Gradient();	
		Buzzer_VoiceFeedback_2();	
		
		Menu_Mode_Num=2;
		RGB_Mode_Num=1;
	}
		
	else if (Voice_Mode_Num == 18)
	{
		OLED_Clear_One();
		OLED_Show_Voice_All_ON();
		Buzzer_VoiceFeedback_2();
        
		Dimming_Mode_Num=3;
		Dimming ();
		RGB_Mode_Num=1;
		Menu_Mode_Num=2;
	}
		
	else if (Voice_Mode_Num == 19)
	{
		OLED_Clear_One();
		OLED_Show_Voice_All_OFF();
		Buzzer_VoiceFeedback_2();	
		
		Dimming_Mode_Num=1;
		Dimming ();
		RGB_Mode_Num=8;
		Menu_Mode_Num=2;
	}
}


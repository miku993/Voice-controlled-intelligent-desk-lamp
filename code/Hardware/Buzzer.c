#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//********************************************初始化
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);      //使能或失能APB2外设时钟
	GPIO_InitTypeDef GPIO_InitStructure;      //定义结构体
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP ;    //初始化GPIO输出模式--推挽输出
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;        //初始化GIOP输出引脚
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;     //初始化GPIO输出频率
	GPIO_Init (GPIOB ,&GPIO_InitStructure);    //GPIO端口初始化
	
	GPIO_ResetBits (GPIOB ,GPIO_Pin_6);
}

//***********************************************电平反转
void Buzzer_Turn(void)
{
    if(GPIO_ReadOutputDataBit(GPIOB ,GPIO_Pin_6)==0)	
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	}
}

//*************************************************开
void Buzzer_ON(void)
{
	GPIO_SetBits (GPIOB ,GPIO_Pin_6);
}

//*************************************************关
void Buzzer_OFF(void)
{
	GPIO_ResetBits (GPIOB ,GPIO_Pin_6);
}

//*************************************************语音反馈1
void Buzzer_VoiceFeedback_1(void)
{
	Buzzer_ON ();
	Delay_ms (100);
	Buzzer_OFF ();
	Delay_ms (100);
}

//*************************************************语音反馈2
void Buzzer_VoiceFeedback_2(void)
{
	Buzzer_ON ();
	Delay_ms (100);
	Buzzer_OFF ();
	Delay_ms (100);
	
	Buzzer_ON ();
	Delay_ms (100);
	Buzzer_OFF ();
	Delay_ms (100);
}


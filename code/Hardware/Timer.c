#include "stm32f10x.h"                  // Device header
#include "PWM.h"

u8 Time_RGB=0;
u8 Time_Regular=0;
u8 Time_Voice=1;

//*****************************************************************Tim3初始化
void TIM3_Init(void)
{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3 ,ENABLE );
	
	TIM_InternalClockConfig(TIM3);  //选择内部时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;  //100ms=0.1s
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3 ,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE );  //使能更新中断
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );  //配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init (&NVIC_InitStructure);
}



//*****************************************************************开启定时器3
void TIM3_Open(void)
{
	TIM_Cmd(TIM3,ENABLE );
}

//*****************************************************************关闭定时器3
void TIM3_Close(void)
{
	TIM_Cmd(TIM3,DISABLE );
}


//*****************************************************************中断3
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus (TIM3,TIM_IT_Update) == SET)  //如果更新中断标志位==1
	{
		Time_Voice ++;
		Time_RGB ++;
		TIM_ClearITPendingBit (TIM3,TIM_IT_Update);  //清除标志位
	}
	else if(Time_RGB == 2)
	{
		Time_RGB=0;
	}
	else if(Time_Voice == 10)
	{
		Time_Voice=0;
	}
}


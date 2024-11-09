#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "RGB.h"

//（1）开启时钟
//（2）选择时基单元时钟
//（3）配置时基单元
//（4）配置比较寄存器
//（5）初始化GPIO
//（6）启动定时器

//*********************************************************初始化
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2 ,ENABLE );
	
	TIM_InternalClockConfig (TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;  //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;  //PSC  -->1ms
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit (TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);  //所有成员赋一个初值（因为所有模式都需要配置）
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  //输出比较的模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置CCR的值
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);
	
    RGB_Init();
	LED_Init();
   
	
	TIM_Cmd(TIM2,ENABLE );  //（6）
}

//*********************************************************LED比较通道
void PWM_SetCompare1(u8 Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

//*********************************************************Red比较通道
void PWM_SetCompare2(u8 Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}

//*********************************************************Green比较通道
void PWM_SetCompare3(u8 Compare)
{
	TIM_SetCompare3(TIM2,Compare);
}

//*********************************************************Blue比较通道
void PWM_SetCompare4(u8 Compare)
{
	TIM_SetCompare4(TIM2,Compare);
}

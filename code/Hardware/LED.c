#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//*******************************************************************初始化
void LED_Init(void)  //配合PWM
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE );
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用开漏/推挽输出模式 -- 将控制权交给定时器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&GPIO_InitStructure);
	
	GPIO_SetBits (GPIOA ,GPIO_Pin_0);
}

//*******************************************************************人体红外感应
void LED_Switch_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB ,ENABLE );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO ,ENABLE );   //EXTI不需要开启时钟，NVIC是内核外设，同样不需要开时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD ;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init (GPIOB ,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB ,GPIO_PinSource5);
    
    EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line= EXTI_Line4;   //中断线路
    EXTI_InitStructure.EXTI_LineCmd= ENABLE;   //允许中断
	EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising;  //下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;  //中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure); 
	
}


  




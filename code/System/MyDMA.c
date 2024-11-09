#include "stm32f10x.h"                  // Device header

void MyDMA_Init(uint32_t Addr)
{
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE );
    
	DMA_DeInit (DMA1_Channel1);  //设置复位
	
	DMA_InitTypeDef DMA_InitStruct;
	//********************************外设站点
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t )(&TIM4->CCR1 );  //外设站点地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //位宽
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //是否自增
	//********************************存储器站点	DMA_InitStruct.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_MemoryDataSize = Addr;
	//********************************
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;  //传输方向(DST--destination; SRC--source)
	DMA_InitStruct.DMA_BufferSize = 0;  //传输计数器
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;  //传输模式--单次
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  //软件触发&硬件触发
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;  //优先级
	DMA_Init (DMA1_Channel1,&DMA_InitStruct);
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}


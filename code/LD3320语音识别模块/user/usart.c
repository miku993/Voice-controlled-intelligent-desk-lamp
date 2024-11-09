
#include "config.h"
#define FOSC 22118400L      //System frequency
uint32_t baud=9600;           //UART baudrate

uint8_t RX_BUF[50];
uint8_t NUM_1=0;
/************************************************************************
函 数 名： 串口初始化
功能描述： STC10L08XE 单片机串口初始化函数
返回函数： none
其他说明： none
**************************************************************************/
void UartIni(void)
{
    SCON = 0x50;            //8-bit variable UART
    TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/baud); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}
/************************************************************************
功能描述： 	串口发送一字节数据
入口参数：	DAT:带发送的数据
返 回 值： 	none
其他说明：	none
**************************************************************************/
void UARTSendByte(uint8_t DAT)
{
	ES  =  0;
	TI=0;
	SBUF = DAT;
	while(TI==0);
	TI=0;
	ES = 1;
}
/************************************************************************
功能描述： 串口发送字符串数据
入口参数： 	*DAT：字符串指针
返 回 值： none
其他说明： API 供外部使用，直观！
**************************************************************************/
void PrintCom(uint8_t *DAT)
{
	while(*DAT)
	{
	 	UARTSendByte(*DAT++);
	}	
}

void Uart_Isr() interrupt 4 using 1
{
    if (RI)
    {
			RI=0;
			RX_BUF[NUM_1]=SBUF;
			NUM_1++;
			if(NUM_1>=49)
				NUM_1=0;
			if(NUM_1>=3)
			{
				if(RX_BUF[NUM_1]==0xF8&&RX_BUF[NUM_1-1]==0xF8&&RX_BUF[NUM_1-2]==0xF8)
					IAP_CONTR=0x60;		
			}		
		}
}
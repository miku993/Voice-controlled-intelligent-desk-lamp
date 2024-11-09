/*******************************************************
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**  延时口令模式： 即识别时都说“小杰”这个口令唤醒后，如果15秒内不进行语音识别则退出唤醒
/*********************************************************/
#include "config.h"

#define LD_INDEX_PORT		(*((volatile unsigned char xdata*)(0x8100)))
#define LD_DATA_PORT		(*((volatile unsigned char xdata*)(0x8000)))

void LD_WriteReg( unsigned char address, unsigned char dataout )
{
  LD_INDEX_PORT  = address;
  LD_DATA_PORT = dataout;
}

unsigned char LD_ReadReg( unsigned char address )
{
  LD_INDEX_PORT = address;
  return (unsigned char)LD_DATA_PORT;
}


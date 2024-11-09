#include "stm32f10x.h"
#include "OLED_Font.h"
#include "Delay.h"

/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(x))

extern u8 Clear;

/*引脚初始化*/
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
	    for(i = 0; i < 128; i++)
	    {
		    OLED_WriteData(0x00);
	    }
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}



/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}

//**************************************************************清屏一次函数
void OLED_Clear_One(void)  
{      	
	if(Clear==1)
    {
	    Clear=0;
	    OLED_Clear ();
	}
}

//**************************************************************中文显示
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num)  
{      	
	uint8_t i;
	uint8_t wide=20;//字宽
	
	OLED_SetCursor((Line-1)*2, (Column-1)*wide);		//参数1:把光标设置在第几页. 参数2:把光标设置在第几列
	for (i = 0; i < wide; i++)
	{
		OLED_WriteData(OLED_F10x16[Num][i]);			//显示上半部分内容
	}
	
	OLED_SetCursor((Line-1)*2+1,(Column-1)*wide);		
	for (i = 0; i < wide; i++)
	{
		OLED_WriteData(OLED_F10x16[Num][i+wide]);		//显示下半部分内容
	}
 
}

//**************************************************************办公模式
void OLED_Show_LED(void)  
{      
	OLED_ShowCN (1,2,0);  //办公模式
    OLED_ShowCN (1,3,1);
    OLED_ShowCN (1,4,2);
    OLED_ShowCN (1,5,3);
		
	OLED_ShowCN (2,2,12);  //当前亮度
	OLED_ShowCN (2,3,13);
	OLED_ShowCN (2,4,14);
	OLED_ShowCN (2,5,15);
		
	OLED_ShowCN (4,1,26);  //加
    OLED_ShowCN (4,6,27);  //减
}

//**************************************************************定时模式
void OLED_Show_Regular(void)  
{      
	OLED_ShowCN (1,2,0);  //办公模式
    OLED_ShowCN (1,3,1);
    OLED_ShowCN (1,4,2);
    OLED_ShowCN (1,5,3);
		
	OLED_ShowCN (2,2,12);  //当前亮度
	OLED_ShowCN (2,3,13);
	OLED_ShowCN (2,4,14);
	OLED_ShowCN (2,5,15);
		
	OLED_ShowCN (4,1,26);  //加
    OLED_ShowCN (4,6,27);  //减
}

//**************************************************************氛围模式
void OLED_Show_RGB(void)  
{      
	OLED_ShowCN (1,2,0);  //办公模式
    OLED_ShowCN (1,3,1);
    OLED_ShowCN (1,4,2);
    OLED_ShowCN (1,5,3);
		
	OLED_ShowCN (2,2,12);  //当前亮度
	OLED_ShowCN (2,3,13);
	OLED_ShowCN (2,4,14);
	OLED_ShowCN (2,5,15);
		
	OLED_ShowCN (4,1,26);  //加
    OLED_ShowCN (4,6,27);  //减
}

//**************************************************************设置--办公模式
void OLED_Show_Menu_LED(void)  
{      
    OLED_ShowCN (1,3,28);  //设置
	OLED_ShowCN (1,4,29);
		
	OLED_ShowCN (2,2,0);  //办公模式
	OLED_ShowCN (2,3,1);
	OLED_ShowCN (2,4,2);
	OLED_ShowCN (2,5,3);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
	OLED_ShowString (4,14,">>");
}

//**************************************************************设置--智能提醒
void OLED_Show_Menu_Regular(void)  
{      
    OLED_ShowCN (1,3,28);  //设置
	OLED_ShowCN (1,4,29);
		
	OLED_ShowCN (2,2,51);  //智能提醒
	OLED_ShowCN (2,3,52);
	OLED_ShowCN (2,4,53);
	OLED_ShowCN (2,5,54);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
	OLED_ShowString (4,14,">>");
}

//**************************************************************设置--氛围模式
void OLED_Show_Menu_RGB(void)  
{      
    OLED_ShowCN (1,3,28);  //设置
	OLED_ShowCN (1,4,29);
		
	OLED_ShowCN (2,2,4);  //氛围模式
	OLED_ShowCN (2,3,5);
	OLED_ShowCN (2,4,2);
	OLED_ShowCN (2,5,3);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--红色
void OLED_Show_RGB_Red(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
	OLED_ShowCN (2,3,35);  //红色
	OLED_ShowCN (2,4,44);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--绿色
void OLED_Show_RGB_Green(void)  
{      	
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
	OLED_ShowCN (2,3,36);  //绿色
	OLED_ShowCN (2,4,44);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--蓝色
void OLED_Show_RGB_Blue(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
	OLED_ShowCN (2,3,37);  //蓝色
	OLED_ShowCN (2,4,44);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--黄色
void OLED_Show_RGB_Yellow(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
	OLED_ShowCN (2,3,41);  //黄色
	OLED_ShowCN (2,4,44);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--紫色
void OLED_Show_RGB_Purple(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
	OLED_ShowCN (2,3,39);  //紫色
	OLED_ShowCN (2,4,44);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--青色
void OLED_Show_RGB_Cyan(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
	OLED_ShowCN (2,3,40);  //蓝色
	OLED_ShowCN (2,4,44);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--渐变色
void OLED_Show_RGB_Gradient(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
 	OLED_ShowCN (2,2,47);  //全色渐变
	OLED_ShowCN (2,3,44);
	OLED_ShowCN (2,4,42);
	OLED_ShowCN (2,5,43);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--关闭
void OLED_Show_RGB_Close(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
 	OLED_ShowCN (2,3,45);  //关闭
	OLED_ShowCN (2,4,46);
		
	OLED_ShowString (4,1,"<<");
	OLED_ShowCN (4,3,14);  //亮度
	OLED_ShowCN (4,4,15);
	OLED_ShowString (4,14,">>");
}

//**************************************************************氛围模式--亮度调节
void OLED_Show_RGB_Dimming(void)  
{      
	OLED_ShowCN (1,2,4);  //氛围模式
	OLED_ShowCN (1,3,5);
	OLED_ShowCN (1,4,2);
	OLED_ShowCN (1,5,3);
	
 	OLED_ShowCN (2,2,12);  //当前亮度
	OLED_ShowCN (2,3,13);
	OLED_ShowCN (2,4,14);
	OLED_ShowCN (2,5,15);
		
	OLED_ShowCN (4,1,26);  //加
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
    OLED_ShowCN (4,6,27);  //减
	
}

//**************************************************************智能提醒--定时打开
void OLED_Show_Regular_Open(void)  
{      
	OLED_ShowCN (1,2,51);  //定时模式
	OLED_ShowCN (1,3,52);
	OLED_ShowCN (1,4,53);
	OLED_ShowCN (1,5,54);
	
 	OLED_ShowCN (2,2,8);  //定时打开
	OLED_ShowCN (2,3,9);
	OLED_ShowCN (2,4,50);
	OLED_ShowCN (2,5,32);
		
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
	OLED_ShowString(4,14,">>");
}

//**************************************************************智能提醒--定时关闭		
void OLED_Show_Regular_Close(void)  
{      
	OLED_ShowCN (1,2,51);  //定时模式
	OLED_ShowCN (1,3,52);
	OLED_ShowCN (1,4,53);
	OLED_ShowCN (1,5,54);
	
 	OLED_ShowCN (2,2,8);  //定时关闭
	OLED_ShowCN (2,3,9);
	OLED_ShowCN (2,4,45);
	OLED_ShowCN (2,5,46);
		
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
	OLED_ShowString(4,14,">>");
}

//**************************************************************智能提醒--定时打开--分
void OLED_Show_Regular_Open_Min(void)  
{      
	OLED_ShowCN (1,2,51);  //定时模式
	OLED_ShowCN (1,3,52);
	OLED_ShowCN (1,4,53);
	OLED_ShowCN (1,5,54);
	
 	OLED_ShowCN (2,4,33);  //分
		
	OLED_ShowCN (4,1,26);  //加
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
    OLED_ShowCN (4,6,27);  //减
}

//**************************************************************定时模式--定时打开--秒
void OLED_Show_Regular_Open_Sec(void)  
{      
	OLED_ShowCN (1,2,51);  //定时模式
	OLED_ShowCN (1,3,52);
	OLED_ShowCN (1,4,53);
	OLED_ShowCN (1,5,54);
	
 	OLED_ShowCN (2,4,34);  //秒
		
	OLED_ShowCN (4,1,26);  //加
	OLED_ShowCN (4,3,24);  //确定
	OLED_ShowCN (4,4,25);
    OLED_ShowCN (4,6,27);  //减
}

//**************************************************************语音识别--我在
void OLED_Show_Voice_Yes(void)  
{      
	OLED_ShowCN (2,3,55);  //我在
	OLED_ShowCN (2,4,56);
}

//**************************************************************语音识别--没说话
void OLED_Show_Voice_No(void)  
{      
	OLED_ShowCN (2,1,67);  //您好像没说话
	OLED_ShowCN (2,2,68);
	OLED_ShowCN (2,3,69);
	OLED_ShowCN (2,4,70);
	OLED_ShowCN (2,5,71);
	OLED_ShowCN (2,6,72);
}

//**************************************************************语音识别--灯已打开
void OLED_Show_Voice_LED_ON(void)  
{      
	OLED_ShowCN (2,2,57);  //灯已打开
	OLED_ShowCN (2,3,58);
	OLED_ShowCN (2,4,50);
	OLED_ShowCN (2,5,18);
}

//**************************************************************语音识别--灯已关闭
void OLED_Show_Voice_LED_OFF(void)  
{      
	OLED_ShowCN (2,2,57);  //灯已关闭
	OLED_ShowCN (2,3,58);
	OLED_ShowCN (2,4,45);
	OLED_ShowCN (2,5,46);
}

//**************************************************************语音识别--亮度增加
void OLED_Show_Voice_LED_Up(void)  
{      
	OLED_ShowCN (2,2,48);  //调节成功
	OLED_ShowCN (2,3,49);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--亮度减小
void OLED_Show_Voice_LED_Low(void)  
{      
	OLED_ShowCN (2,2,48);  //调节成功
	OLED_ShowCN (2,3,49);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--亮度一挡
void OLED_Show_Voice_LED_One(void)  
{      
	OLED_ShowCN (2,2,48);  //调节成功
	OLED_ShowCN (2,3,49);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--亮度二挡
void OLED_Show_Voice_LED_Two(void)  
{      
	OLED_ShowCN (2,2,48);  //调节成功
	OLED_ShowCN (2,3,49);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--亮度三挡
void OLED_Show_Voice_LED_Three(void)  
{      
	OLED_ShowCN (2,2,48);  //调节成功
	OLED_ShowCN (2,3,49);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--亮度四挡
void OLED_Show_Voice_LED_Four(void)  
{      
	OLED_ShowCN (2,2,48);  //调节成功
	OLED_ShowCN (2,3,49);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}


//**************************************************************语音识别--打开氛围模式
void OLED_Show_Voice_Mode_RGB_ON(void)  
{      
	OLED_ShowCN (2,2,57);  //灯已打开
	OLED_ShowCN (2,3,58);
	OLED_ShowCN (2,4,50);
	OLED_ShowCN (2,5,18);
}

//**************************************************************语音识别--关闭氛围模式
void OLED_Show_Voice_Mode_RGB_OFF(void)  
{      
	OLED_ShowCN (2,2,57);  //灯已关闭
	OLED_ShowCN (2,3,58);
	OLED_ShowCN (2,4,45);
	OLED_ShowCN (2,5,46);
}

//**************************************************************语音识别--红色效果
void OLED_Show_Voice_RGB_Red(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--绿色效果
void OLED_Show_Voice_RGB_Green(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}


//**************************************************************语音识别--蓝色效果
void OLED_Show_Voice_RGB_Blue(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}


//**************************************************************语音识别--黄色效果
void OLED_Show_Voice_RGB_Yellow(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}


//**************************************************************语音识别--紫色效果
void OLED_Show_Voice_RGB_Purple(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}


//**************************************************************语音识别--青色效果
void OLED_Show_Voice_RGB_Cyan(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}


//**************************************************************语音识别--渐变色效果
void OLED_Show_Voice_RGB_Gradient(void)  
{      
	OLED_ShowCN (2,2,65);  //切换成功
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--全部打开
void OLED_Show_Voice_All_ON(void)  
{      
	OLED_ShowCN (2,1,58);  //已全部打开
	OLED_ShowCN (2,2,65);
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}

//**************************************************************语音识别--全部关闭
void OLED_Show_Voice_All_OFF(void)  
{      
	OLED_ShowCN (2,1,58);  //已全部关闭
	OLED_ShowCN (2,2,65);
	OLED_ShowCN (2,3,66);
	OLED_ShowCN (2,4,59);
	OLED_ShowCN (2,5,60);
}



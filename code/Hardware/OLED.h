#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_Clear_One(void);
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num);
void OLED_Show_LED(void);
void OLED_Show_Regular(void);
void OLED_Show_RGB(void); 
void OLED_Show_Menu_LED(void);
void OLED_Show_Menu_RGB(void);
void OLED_Show_Menu_Regular(void);
void OLED_Show_RGB_Red(void);
void OLED_Show_RGB_Green(void);
void OLED_Show_RGB_Blue(void);
void OLED_Show_RGB_Yellow(void);
void OLED_Show_RGB_Purple(void);
void OLED_Show_RGB_Cyan(void);
void OLED_Show_RGB_Gradient(void);
void OLED_Show_RGB_Close(void);
void OLED_Show_RGB_Dimming(void);
void OLED_Show_Regular_Open(void);  
void OLED_Show_Regular_Close(void); 
void OLED_Show_Regular_Open_Min(void);
void OLED_Show_Regular_Open_Sec(void);
void OLED_Show_Voice_Yes(void);  
void OLED_Show_Voice_No(void);  
void OLED_Show_Voice_LED_ON(void);  
void OLED_Show_Voice_LED_OFF(void);  
void OLED_Show_Voice_LED_Up(void);  
void OLED_Show_Voice_LED_Low(void);  
void OLED_Show_Voice_LED_One(void);  
void OLED_Show_Voice_LED_Two(void);  
void OLED_Show_Voice_LED_Three(void);  
void OLED_Show_Voice_LED_Four(void);   
void OLED_Show_Voice_Mode_RGB_ON(void);  
void OLED_Show_Voice_Mode_RGB_OFF(void);  
void OLED_Show_Voice_RGB_Red(void);  
void OLED_Show_Voice_RGB_Green(void);  
void OLED_Show_Voice_RGB_Blue(void);  
void OLED_Show_Voice_RGB_Yellow(void);  
void OLED_Show_Voice_RGB_Purple(void);  
void OLED_Show_Voice_RGB_Cyan(void); 
void OLED_Show_Voice_RGB_Gradient(void);  
void OLED_Show_Voice_All_ON(void);  
void OLED_Show_Voice_All_OFF(void);  




#endif

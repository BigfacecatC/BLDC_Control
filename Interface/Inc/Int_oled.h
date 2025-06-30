#ifndef __OLED_H
#define __OLED_H

#include "gpio.h"
#include "i2c.h"
#include "stdlib.h"

// I2C地址
#define OLED_I2C_ADDRESS 0x78

#define OLED_CMD         0 // 命令
#define OLED_DATA        1 // 数据

void Int_OLED_ColorTurn(uint8_t i);
void Int_OLED_DisplayTurn(uint8_t i);
void Int_OLED_WR_Byte(uint8_t dat, uint8_t mode);
void Int_OLED_DisPlay_On(void);
void Int_OLED_DisPlay_Off(void);
void Int_OLED_Refresh(void);
//若同一位置字符有切换，则要调用该函数，不然显示会重叠
void Int_OLED_Clear(void);

//显示字符时调用显示字符串可以得到完全一致的效果
//显示英文字符时8*16的字符，显示时占用8个像素宽
void Int_OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode);
void Int_OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1, uint8_t mode);
void Int_OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode);
void Int_OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode);
void Int_OLED_ScrollDisplay(uint8_t num, uint8_t space, uint8_t mode);
void Int_OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode);
void Int_OLED_Init(void);

#endif

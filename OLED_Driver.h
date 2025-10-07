#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#define Uchar unsigned char
#define Uint  unsigned int
#define Ulong unsigned long
#define uchar unsigned char
#define uint  unsigned int
void 	delay_ms(Uint t);

//	oled初始化函数
void OLED_Init(void);
// 设置颜色模式
void OLED_SetColorMode(bool colormode);
//	oled全局刷新函数
void OLED_Update(void);
//	oled局部刷新函数
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
// OLED 设置亮度函数
void OLED_Brightness(int16_t Brightness);
void ff_display3(void);
#endif

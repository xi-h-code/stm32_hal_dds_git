#ifndef __AD9833_H__
#define __AD9833_H__

#include "main.h"

// 定义晶振频率 (常用模块为 25MHz)
#define AD9833_MCLK         25000000UL 

// 波形类型枚举
typedef enum {
    WAVE_SINE     = 0x2000, // 正弦波
    WAVE_TRIANGLE = 0x2002, // 三角波
    WAVE_SQUARE   = 0x2028, // 方波 (MSB)
} AD9833_Wave_t;

// FSYNC 引脚控制 (假设在 CubeMX 中命名为 FSYNC，对应 PA4)
#define AD9833_FSYNC_0()    HAL_GPIO_WritePin(FSYNC_GPIO_Port, FSYNC_Pin, GPIO_PIN_RESET)
#define AD9833_FSYNC_1()    HAL_GPIO_WritePin(FSYNC_GPIO_Port, FSYNC_Pin, GPIO_PIN_SET)

// 函数声明
void AD9833_Init(void);
void AD9833_SetFrequency(double freq, AD9833_Wave_t wave);
void AD9833_WriteRegister(uint16_t data);

#endif

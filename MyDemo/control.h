#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "ad9833.h"

// 外部可调用的接口
void Key_Init(void);
void Key_Process(void);
void UI_Refresh(double freq, AD9833_Wave_t wave);

#endif
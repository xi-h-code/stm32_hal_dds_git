#include "control.h"
#include "blue.h"
#include <stdio.h>
#include <string.h>

// 静态全局变量，用来保存当前状态，不对外暴露
static double current_freq = 1000.0;           
static AD9833_Wave_t current_wave = WAVE_SINE; 

/**
 * @brief 刷新屏幕 UI 显示
 */
void UI_Refresh(double freq, AD9833_Wave_t wave) 
{
    char *wave_str;
    if (wave == WAVE_SINE)           wave_str = "Sine    ";
    else if (wave == WAVE_TRIANGLE)  wave_str = "Triangle";
    else                             wave_str = "Square  ";

    printf("[d-c]"); // 清屏
    HAL_Delay(10);
    printf("[d,10,20,Wave: %s,24]", wave_str);
    printf("[d,10,60,Freq: %.1f Hz,24]", freq);
}

/**
 * @brief 业务逻辑初始化（设置默认波形和刷新初始屏幕）
 */
void Key_Init(void) 
{
    // 初始化状态：1kHz 正弦波
    current_freq = 1000.0;
    current_wave = WAVE_SINE;
    
    // 下发给硬件
    AD9833_SetFrequency(current_freq, current_wave);
    
    // 稍微等待屏幕准备好后刷新 UI
    HAL_Delay(100);
    UI_Refresh(current_freq, current_wave);
}

/**
 * @brief 按键与串口指令处理任务，需放在主循环 while(1) 中
 */
void Key_Process(void) 
{
    if (Serial_RxFlag == 1) 
    {
        int key_num = 0;
        char action[10] = {0}; 
        
        // 解析串口传来的蓝牙指令
        if (sscanf((char*)Serial_RxPacket, "key,%d,%9s", &key_num, action) == 2) 
        {
            if (strcmp(action, "down") == 0) 
            {
                switch (key_num) 
                {
                    case 1: current_freq = 1000.0; current_wave = WAVE_SINE; break;
                    case 2: current_freq = 1000.0; current_wave = WAVE_TRIANGLE; break;
                    case 3: current_freq = 1000.0; current_wave = WAVE_SQUARE; break;
                    case 4: // 轮询切换波形
                        if (current_wave == WAVE_SINE) current_wave = WAVE_TRIANGLE;
                        else if (current_wave == WAVE_TRIANGLE) current_wave = WAVE_SQUARE;
                        else current_wave = WAVE_SINE;
                        break;
                    case 5: current_freq += 1000.0; break;
                    case 6: current_freq -= 1000.0; break;
                    case 7: current_freq += 10.0;   break;
                    case 8: current_freq -= 10.0;   break;
                    case 9: current_freq = 1000000.0; current_wave = WAVE_SINE; break;
                    case 10: current_freq = 10.0;      current_wave = WAVE_SINE; break;
                }

                // 频率范围限制 10Hz ~ 1MHz
                if (current_freq > 1000000.0) current_freq = 1000000.0;
                if (current_freq < 10.0)      current_freq = 10.0;

                // 驱动硬件并刷新屏幕
                AD9833_SetFrequency(current_freq, current_wave);
                UI_Refresh(current_freq, current_wave); 
            }
        }
        
        // 处理完毕，清除标志位，等待下一次中断
        Serial_RxFlag = 0;
    }
}
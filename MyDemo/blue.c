#include "blue.h"
#include "usart.h" 
#include <stdio.h>

char Serial_RxPacket[100];
volatile uint8_t Serial_RxFlag = 0;
uint8_t RxTemp; 

void Serial_Init(void) {
    HAL_UART_Receive_IT(&huart1, &RxTemp, 1);
}

// 重定向 printf 到串口，用于 UI 刷新
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;

    if (huart->Instance == USART1) {
        if (RxState == 0) {
            if (RxTemp == '[' && Serial_RxFlag == 0) {
                RxState = 1;
                pRxPacket = 0;
            }
        } else if (RxState == 1) {
            if (RxTemp == ']') {
                RxState = 0;
                Serial_RxPacket[pRxPacket] = '\0';
                Serial_RxFlag = 1;
            } else {
                if(pRxPacket < 99) Serial_RxPacket[pRxPacket++] = RxTemp;
            }
        }
        HAL_UART_Receive_IT(&huart1, &RxTemp, 1);
    }
}

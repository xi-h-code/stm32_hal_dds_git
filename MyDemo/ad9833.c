#include "ad9833.h"

extern SPI_HandleTypeDef hspi1; // 需在 CubeMX 中配置为 16-Bit Data Size

void AD9833_WriteRegister(uint16_t data) {
    AD9833_FSYNC_0(); 
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 1, HAL_MAX_DELAY);
    AD9833_FSYNC_1(); 
}

void AD9833_Init(void) {
    AD9833_FSYNC_1();
    AD9833_WriteRegister(0x2100); // 复位寄存器
}

void AD9833_SetFrequency(double freq, AD9833_Wave_t wave) {
    // 2^28 / 25MHz = 10.73741824
    unsigned long long freq_val = (unsigned long long)(freq * 10.73741824);

    uint16_t freq_LSB = (uint16_t)(freq_val & 0x3FFF);         
    uint16_t freq_MSB = (uint16_t)((freq_val >> 14) & 0x3FFF); 

    AD9833_WriteRegister(0x2100); 
    AD9833_WriteRegister(0x4000 | freq_LSB); // 写入 FREQ0 低14位
    AD9833_WriteRegister(0x4000 | freq_MSB); // 写入 FREQ0 高14位
    AD9833_WriteRegister(0xC000);            // 相位设为 0
    AD9833_WriteRegister(wave);              // 退出复位并输出波形
}


#ifndef __BLUE_H
#define __BLUE_H

#include "main.h"

extern char Serial_RxPacket[];
extern volatile uint8_t Serial_RxFlag;

void Serial_Init(void);
void Serial_SendString(char *String);

#endif

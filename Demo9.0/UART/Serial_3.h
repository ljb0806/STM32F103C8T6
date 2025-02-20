#ifndef __SERIAL_3_H
#define __SERIAL_3_H

#include <stdio.h>

void Serial_Init_3(void);
void Serial_SendByte_3(uint8_t Byte);
void Serial_SendString_3(char *String);
void Serial_SendArray_3(uint8_t *Array, uint16_t Length);
void Serial_SendNum_3(uint32_t Number, uint8_t Length);
uint32_t Serial_Pow_3(uint32_t X, uint32_t Y);
uint8_t Serial_GetRxFlag_3(void);
uint8_t Serial_GetRxData_3(void);

#endif

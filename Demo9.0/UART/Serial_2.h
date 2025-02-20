#ifndef __SERIAL_2_H
#define __SERIAL_2_H

#include <stdio.h>

extern uint8_t Serial_RxData_2;         // 从 串口屏 处接收的数据
extern uint8_t Serial_RxFlag_2;         // 接收完的标志

void Serial_Init_2(void);
void Serial_SendByte_2(uint8_t Byte);
void Serial_SendString_2(char *String);
void Serial_SendArray_2(uint8_t *Array, uint16_t Length);
void Serial_SendNum_2(uint32_t Number, uint8_t Length);
uint32_t Serial_Pow_2(uint32_t X, uint32_t Y);
uint8_t Serial_GetRxFlag_2(void);
uint8_t Serial_GetRxData_2(void);

#endif

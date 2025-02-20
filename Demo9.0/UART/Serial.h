#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Receive_Data[4];
extern uint8_t Serial_RxData_1;         // 从 M0 处接收的数据
extern uint8_t Serial_RxFlag_1;         // 接收完的标志


void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendNum(uint32_t Number, uint8_t Length);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif

#ifndef __SCCB_H
#define __SCCB_H
#include "stm32f10x.h"

#define GPIO_PORT GPIOB
#define SCL GPIO_Pin_10
#define SDA GPIO_Pin_11
#define OV7670_ADDRESS 0x42

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_SendAck(void);
void SCCB_W_SCL(uint8_t BitValue);
void SCCB_W_SDA(uint8_t BitValue);
void SCCB_SendByte(uint8_t SendByte);
uint8_t SCCB_ReceiveByte(void);
uint8_t SCCB_ReceiveAck(void);
uint8_t SCCB_R_SDA(void);

#endif

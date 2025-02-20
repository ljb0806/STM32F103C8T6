#ifndef __I2C_Software_H
#define __I2C_Software_H

void I2C_Software_Init(void);
void I2C_Software_Start(void);
void I2C_Software_Stop(void);
void I2C_Software_SendByte(uint8_t Byte);
uint8_t I2C_Software_ReceiveByte(void);
void I2C_Software_SendAck(uint8_t AckBit);
uint8_t I2C_Software_ReceiveAck(void);

#endif

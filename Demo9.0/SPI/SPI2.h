#include "stm32f10x.h"

#ifndef __SPI2_H
#define __SPI2_H

uint8_t SPI_WriteByte(SPI_TypeDef* SPIx, uint8_t byte);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);
void SPI2_Init(void);

#endif

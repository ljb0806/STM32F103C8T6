#ifndef __OV7670_H
#define __OV7670_H

#include "stm32f10x.h"
#define OV7670_ADDRESS 0x42

uint8_t OV7670_ReadReg(uint8_t reg);
void OV7670_WriteReg(uint8_t reg, uint8_t data);
void OV7670_Init(void);
void OV7670_Special_Effects(u8 eft);
void OV7670_Contrast(u8 contrast);
void OV7670_Light(u8 light);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Window_Set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
void OV7670_Effects_Set(void);


#endif

#ifndef __AD9833_H
#define __AD9833_H
#include "stm32f10x.h"                  // Device header

#define TRI_WAVE 	0  		
#define SIN_WAVE 	1	
#define SQU_WAVE 	2	

#define		FSYNC_1()     	GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define		FSYNC_0()   	GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define     SCK_1()			GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define 	SCK_0()			GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define 	DAT_1()			GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define 	DAT_0()			GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define 	CS_1()			GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define 	CS_0()			GPIO_ResetBits(GPIOB, GPIO_Pin_6)

void AD9833_Init(void);
void AD9833_WaveSeting(double frequence,unsigned int frequence_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_AmpSet(unsigned char amp);

#endif

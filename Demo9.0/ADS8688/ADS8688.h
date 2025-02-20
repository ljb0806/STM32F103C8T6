#ifndef _ADS8688_H_ 
#define _ADS8688_H_ 

#include "stm32f10x.h"

#define ADS_8688_DAISY_IN_H GPIO_SetBits(GPIOA,GPIO_Pin_5) 
#define ADS_8688_DAISY_IN_L GPIO_ResetBits(GPIOA,GPIO_Pin_5) 
#define ADS_8688_SCLK_H GPIO_SetBits(GPIOA,GPIO_Pin_4) 
#define ADS_8688_SCLK_L GPIO_ResetBits(GPIOA,GPIO_Pin_4) 
#define ADS_8688_nCS_H GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define ADS_8688_nCS_L GPIO_ResetBits(GPIOA,GPIO_Pin_7) 
#define ADS_8688_RST_PD_H GPIO_SetBits(GPIOA,GPIO_Pin_2) 
#define ADS_8688_RST_PD_L GPIO_ResetBits(GPIOA,GPIO_Pin_2) 
#define ADS_8688_SDI_H GPIO_SetBits(GPIOA,GPIO_Pin_3) 
#define ADS_8688_SDI_L GPIO_ResetBits(GPIOA,GPIO_Pin_3) 
#define ADS_8688_SDO GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//Command Register 

#define NO_OP 0X0000 
#define STDBY 0X8200 
#define PWR_DN 0X8300 
#define RST 0X8500 
#define AUTO_RST 0XA000 

#define MAN_Ch_1 0XC000 
#define MAN_Ch_2 0XC400 
#define MAN_Ch_3 0XC800 
#define MAN_Ch_4 0XCC00 
#define MAN_Ch_5 0XD000 
#define MAN_Ch_6 0XD400 
#define MAN_Ch_7 0XD800 
#define MAN_Ch_8 0XDC00 
#define MAN_AUX 0XE000 
#define CH1 0x05 
#define CH2 0x06 
#define CH3 0x07 
#define CH4 0x08 
#define CH5 0x09 
#define CH6 0x0a 
#define CH7 0x0b 
#define CH8 0x0c 

void ADS8688_GPIO_Init(void);
void ADS8688_SPI_WB(uint8_t data);
uint8_t ADS8688_SPI_RB(void);
void ADS8688_WriteCommandReg(uint16_t command);
void ADS8688_Write_Program_Register(uint8_t Addr,uint8_t data);
uint8_t ADS8688_READ_Program_Register(uint8_t Addr);
void Enter_RESET_MODE(void);
void AUTO_RST_MODE(void);
void MAN_Ch_n_Mode(uint16_t ch);
void CH_Range_Select(uint8_t ch, uint8_t range);
void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum);
uint16_t Get_MAN_ch_n_Mode_Data(void);
void ADS8688_Init_Mult(void);
uint16_t get_ADS_ch1(void);

#endif

#ifndef __ADS1118_H
#define __ADS1118_H
#include "stm32f10x.h"

#define ADS1118_SS_START        0X8000    //0--》NO effect,1-->start ADC

#define ADS1118_MUX_AIN0_AIN1   0X0000    //000 = AINP is AIN0 and AINN is AIN1 (default)
#define ADS1118_MUX_AIN0_AIN3   0x1000    //001 = AINP is AIN0 and AINN is AIN3
#define ADS1118_MUX_AIN1_AIN3   0X2000    //010 = AINP is AIN1 and AINN is AIN3
#define ADS1118_MUX_AIN2_AIN3   0X3000    //011 = AINP is AIN2 and AINN is AIN3
#define ADS1118_MUX_AIN0        0X4000    //100 = AINP is AIN0 and AINN is GND
#define ADS1118_MUX_AIN1        0X5000    //101 = AINP is AIN1 and AINN is GND
#define ADS1118_MUX_AIN2        0X6000    //110 = AINP is AIN2 and AINN is GND
#define ADS1118_MUX_AIN3        0X7000    //111 = AINP is AIN3 and AINN is GND

#define ADS1118_PGA_6144        0X0000    //000 = FSR is ±6.144 V
#define ADS1118_PGA_4096        0X0200    //001 = FSR is ±4.096 V
#define ADS1118_PGA_2048        0X0400    //010 = FSR is ±2.048 V (default)
#define ADS1118_PGA_1024        0X0600    //011 = FSR is ±1.024 V
#define ADS1118_PGA_0512        0X0800    //100 = FSR is ±0.512 V
#define ADS1118_PGA_0256        0X0A00    //101 = FSR is ±0.256 V

#define ADS1118_Continuous_MODE 0X0000    //0->Continuous
#define ADS1118_Sigle_SHOT_MODE 0X0100    //1->SIGNEL ADC

#define ADS1118_DR_8SPS         0X0000    //000 = 8 SPS
#define ADS1118_DR_16SPS        0X0020    //001 = 16 SPS
#define ADS1118_DR_32SPS        0X0040    //010 = 32 SPS
#define ADS1118_DR_64SPS        0X0060    //011 = 64 SPS
#define ADS1118_DR_128SPS       0X0080    //100 = 128 SPS (default)
#define ADS1118_DR_250SPS       0X00A0    //101 = 250 SPS
#define ADS1118_DR_470SPS       0X00C0    //110 = 475 SPS
#define ADS1118_DR_860SPS       0X00E0    //111 = 860 SPS

#define ADS1118_ADC_MODE        0x0000    //ADC MODE
#define ADS1118_Temp_MODE       0X0010    //Temperature sensor mode

#define ADS1118_PUUP_DIS        0X0000    //inside pullup disabled
#define ADS1118_PUUP_EN         0x0008    //inside pullup enabled

#define ADS1118_NOP_UPDATA      0X0002    //update the Config register

void MySPI_Init(void);
uint16_t MySPI_SwapByte(uint16_t ByteSend);
void ADS1118_ADC_Init(uint16_t channel,uint16_t PGA,uint16_t SPS);
float  ADS1118_Get_ADC_Continuous_Data(uint16_t channel,uint16_t PGA,uint16_t SPS);

#endif



/**
*****************************************************************************
*
*  @file    ADS1118.c
*  @brief   本文件的函数可完成以下功能：
*			1.ADS1118的初始化与通信
*           2.读取ADS1118的数据
*
*  @author  binbin
*  @date    2024.10.09
*  @version 2.0
*  @note    接线说明:
*  			PA7 - SPI_MOSI 
* 			PA5 - SPI_SCLK
* 			PA4 - SPI_CS
*           PA6 - SPI_MISO
*			警告:
*			ADS1118在多通道读取时会出现一个通道的数据在另一个通道的情况，需要注意区别
*****************************************************************************
**/

#include "stm32f10x.h"
#include "ADS1118.h"
#include "Delay.h"

void MySPI_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_7; // PA7 - SPI_MOSI    PA5 - SPI_SCLK
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;     // PA4 - SPI_CS
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);                     //拉高CS
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;        //SPI_MISO
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //主机模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;  //十六位数据 

	//这里选择的是ADS1118芯片16位数据传输的方式，就把SPI配置成了十六位数据 

	SPI_InitStructure.SPI_Direction =SPI_Direction_2Lines_FullDuplex;  //双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

uint16_t MySPI_SwapByte(uint16_t ByteSend)
{

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
  
	SPI_I2S_SendData(SPI1, ByteSend);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPI1);
}

void ADS1118_ADC_Init(uint16_t channel,uint16_t PGA,uint16_t SPS)
{
	MySPI_Init();
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低CS开始通信
    Delay_us(5);
    MySPI_SwapByte(ADS1118_SS_START|channel|PGA|ADS1118_Continuous_MODE|SPS|ADS1118_Temp_MODE|ADS1118_PUUP_EN|ADS1118_NOP_UPDATA);//发送寄存器命令，交换数据  初始化
    Delay_us(10);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);//拉高CS停止通信
    Delay_ms(1);
}

float ADS1118_Get_ADC_Continuous_Data(uint16_t channel,uint16_t PGA,uint16_t SPS)
{
        uint16_t ADC_Data = 0;
        float Tempture_data= 0;
        GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低CS开始通信
        Delay_us(1);
	    
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));//DOUT拉低ADS1118数据更新准备完毕
       
        ADC_Data = MySPI_SwapByte(ADS1118_SS_START|channel|PGA|ADS1118_Continuous_MODE|SPS|ADS1118_ADC_MODE|ADS1118_PUUP_EN|ADS1118_NOP_UPDATA);//发送寄存器命令，交换数据
        Delay_us(1);
        GPIO_SetBits(GPIOA,GPIO_Pin_4);//拉高CS停止通信
	     
        if(ADC_Data&0X8000)//MSB=1
        {
				ADC_Data = (~ADC_Data)+1 ;
				if(PGA == ADS1118_PGA_6144)Tempture_data=ADC_Data*0.1875;
				else if(PGA == ADS1118_PGA_6144)Tempture_data=ADC_Data*0.1875;
				else if(PGA == ADS1118_PGA_4096)Tempture_data=ADC_Data*0.125;
				else if(PGA == ADS1118_PGA_2048)Tempture_data=ADC_Data*0.0625;
				else if(PGA == ADS1118_PGA_1024)Tempture_data=ADC_Data*0.03125;
				else if(PGA == ADS1118_PGA_0512)Tempture_data=ADC_Data*0.015625;
				else Tempture_data=ADC_Data*0.0078125;
				return Tempture_data;
        }
        else if(PGA == ADS1118_PGA_6144)Tempture_data=ADC_Data*0.1875;
        else if(PGA == ADS1118_PGA_6144)Tempture_data=ADC_Data*0.1875;
        else if(PGA == ADS1118_PGA_4096)Tempture_data=ADC_Data*0.125;
        else if(PGA == ADS1118_PGA_2048)Tempture_data=ADC_Data*0.0625;
        else if(PGA == ADS1118_PGA_1024)Tempture_data=ADC_Data*0.03125;
        else if(PGA == ADS1118_PGA_0512)Tempture_data=ADC_Data*0.015625;
        else Tempture_data=ADC_Data*0.0078125;
        return Tempture_data/1000;
} 











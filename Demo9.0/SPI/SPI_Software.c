/**
*****************************************************************************
*
*  @file    SPI_Software.c
*  @brief   本文件的函数可完成以下功能：
*			1.实现了软件SPI的初始化
*           2.通过软件方式完成字节接收发送
*
*  @author  binbin
*  @date    2024.08.16
*  @version 1.0
*  @note    根据需要可以自由改变宏定义中的引脚与对应接线，和硬件SPI不同，开启总线时钟可能需要修改 <- ！！！！！！！！！！
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header

#define GPIO_PORT            GPIOA          //引脚宏定义
#define MISO_Pin             GPIO_Pin_6
#define MOSI_Pin             GPIO_Pin_7         
#define SCLK                 GPIO_Pin_5
#define CS                   GPIO_Pin_4

void SPI_Software_WriteBit_CS(uint8_t BitValue)                  //写CS引脚
{
	GPIO_WriteBit(GPIO_PORT, CS, (BitAction)BitValue);
}

void SPI_Software_WriteBit_MOSI(uint8_t BitValue)                //写MOSI引脚
{
	GPIO_WriteBit(GPIO_PORT, MOSI_Pin, (BitAction)BitValue);
}

void SPI_Software_WriteBit_SCLK(uint8_t BitValue)                //写SCLK引脚
{
	GPIO_WriteBit(GPIO_PORT, SCLK, (BitAction)BitValue);
}

uint8_t SPI_Software_ReadBit_MISO(void)                          //读MISO引脚
{
	return GPIO_ReadInputDataBit(GPIO_PORT, MISO_Pin);
}	


void SPI_Software_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);         //开启总线时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;                         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //配置位推挽输出
	GPIO_InitStructure.GPIO_Pin = MOSI_Pin | SCLK | CS;          //主发从收 时钟 片选
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                //配置上拉输入
	GPIO_InitStructure.GPIO_Pin = MISO_Pin;                      //主收从发
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT,&GPIO_InitStructure);
	
	SPI_Software_WriteBit_CS(1);                                 //默认CS高电平
	SPI_Software_WriteBit_SCLK(0);                               //默认SCLK低电平
}	

void SPI_Software_Start(void)                                    
{
	SPI_Software_WriteBit_CS(0);                                 //拉低片选启动
}

void SPI_Software_Stop(void)
{
	SPI_Software_WriteBit_CS(1);                                 //拉高片选结束
}

uint8_t SPI_Software_SwapByte(uint8_t ByteSend)                  
{
	for(uint8_t i = 0; i < 8; i++)                               //循环八次
	{
		SPI_Software_WriteBit_MOSI(ByteSend & 0x80);             
		ByteSend <<= 1;                                            
		SPI_Software_WriteBit_SCLK(1);
		if(SPI_Software_ReadBit_MISO() == 1) ByteSend |= 0x01;
		SPI_Software_WriteBit_SCLK(0);                           //按照从高位到低位发出，低位到高位一次读入，节省空间资源的算法
	}
	return ByteSend;
}


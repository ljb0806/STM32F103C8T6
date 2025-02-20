/**
*****************************************************************************
*
*  @file    SPI_Hardware.c
*  @brief   本文件的函数可完成以下功能：
*			1.SPI1的硬件初始化
*           2.SPI1的字节接收发送
*
*  @author  binbin
*  @date    2024.08.16
*  @version 1.0
*  @note    宏定义不可修改，必须按照宏定义引脚接线
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header

#define GPIO_PORT            GPIOA
#define MISO_Pin             GPIO_Pin_6
#define MOSI_Pin             GPIO_Pin_7         
#define SCLK                 GPIO_Pin_5
#define CS                   GPIO_Pin_4

void SPI_Hardware_WriteBit_CS(uint8_t BitValue)                   //只有片选信号CS仍然使用软件方式实现，因为简单
{
	GPIO_WriteBit(GPIO_PORT, CS, (BitAction)BitValue);
}

void SPI_Hardware_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);          //使能总线时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;                          //CS为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               //SCLK MOSI为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = SCLK | MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                 //MISO上拉输入
	GPIO_InitStructure.GPIO_Pin = MISO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT,&GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	                        //STM32的SPI1做主机
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //双线全双工 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       //一帧八个bit
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      //高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;    //时钟为128分频
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                            //按模式0配置
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                              //按模式0配置
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               //CS仍然使用软件方式
	SPI_InitStructure.SPI_CRCPolynomial = 7;                                //无用的默认值
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);                                                  //启动！
	
	SPI_Hardware_WriteBit_CS(1);                                            //片选默认高电平
}

void SPI_Hardware_Start(void)                 //片选拉低，时序开始
{
	SPI_Hardware_WriteBit_CS(0);
}

void SPI_Hardware_Stop(void)                  //片选拉高，时序结束
{
	SPI_Hardware_WriteBit_CS(1);
}

uint8_t SPI_Hardware_SwapByte(uint8_t ByteSend)                    
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);    //等待上次发送完
	SPI_I2S_SendData(SPI1, ByteSend);                                //发送（同时触发接收）
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);   //发送和接收都结束了
	return SPI_I2S_ReceiveData(SPI1);                                //刚好发完收完，直接读取
}

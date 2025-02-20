/**
*****************************************************************************
*
*  @file    Serial_2.c
*  @brief   本文件的函数可完成以下功能：
*			1.UART2的初始化
*           2.UART2发送字节，数组，字符串
*           3.UART2接收字节
*
*  @author  binbin
*  @date    2024.08.15
*  @version 1.0
*
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header
#include "stdio.h"                      // 为了重定向printf函数需要引入该文件
#include "Serial_2.h"
#include "GPIOS.h"

uint8_t Serial_RxData_2;      
uint8_t Serial_RxFlag_2;         

void Serial_Init_2(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      //打开总线上的UART时钟与RX,TX引脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;                        //初始化TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //初始化RX引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;                                             //UART结构体
	USART_InitStructure.USART_BaudRate = 9600;                                         //波特率，可根据要求修改
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    //不需要硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                    //UART的收发模式全打开
	USART_InitStructure.USART_Parity = USART_Parity_No;                                //不用校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                             //1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                        //数据长为8bit
	USART_Init(USART2, &USART_InitStructure);                                          //初始化
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                                     //使能接收中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                    //优先级组别配置位2
	NVIC_InitTypeDef NVIC_InitStructure;                                               //NVIC结构体
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                  //中断通道为USART2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                    //通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                          //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                                 //响应优先级
	NVIC_Init(&NVIC_InitStructure);                                                    //赋值
	
	USART_Cmd(USART2, ENABLE);                                                         //使能UART2开始运行
}

void Serial_SendByte_2(uint8_t Byte)                                                     //通过UART2发送一个字节
{
	USART_SendData(USART2, Byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray_2(uint8_t *Array, uint16_t Length)                                 //发送数组中每一个字节
{
	for (uint16_t i = 0; i < Length; i ++) Serial_SendByte_2(Array[i]);
}

void Serial_SendString_2(char *String)                                                   //发送字符串
{
	for (uint8_t i = 0; String[i] != '\0'; i++) Serial_SendByte_2(String[i]);
}

uint32_t Serial_Pow_2(uint32_t X, uint32_t Y)                                            //计算X的Y次方
{
	uint32_t YY = Y;
	uint32_t Result = 1;
	while(YY--) Result *= X;
	return Result;
}

void Serial_SendNum_2(uint32_t Number, uint8_t Length)                                   //发送数字以字符串形式
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte_2(Number / Serial_Pow_2(10,Length - i - 1) % 10 + '0');
	}
}	

uint8_t Serial_GetRxFlag_2(void)                                                         //发送完成的标志
{
	if(Serial_RxFlag_2 == 1) 
	{
		Serial_RxFlag_2 = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData_2(void)                                                         //获取接收的数据
{
	return Serial_RxData_2;
}

int fputc(int ch, FILE *f)                                                             //重定向printf函数的底层函数
{
	Serial_SendByte_2(ch);
	return ch;
}



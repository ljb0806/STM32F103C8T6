/**
*****************************************************************************
*
*  @file    SCCB.c
*  @brief   本文件的函数可完成以下功能：
*			1.SCCB初始化
*           2.SCCB起始信号
*           3.SCCB停止信号
*           4.SCCB发送一个字节
*           5.SCCB接收一个字节
*           6.SCCB发送应答
*           7.SCCB接收应答
*           8.SCCB写SCL
*           9.SCCB写SDA
*           10.SCCB读SDA
*           
*
*  @author  binbin
*  @date    2024.10.09
*  @version 2.0
*  @note    None
*****************************************************************************
**/

#include "stm32f10x.h"
#include "Delay.h"
#include "SCCB.h"

void SCCB_Init(void)                    //SCCB初始化
{			
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
    GPIO_InitStructure.GPIO_Pin = SDA;				         // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_11);						 // 输出高

	GPIO_InitStructure.GPIO_Pin = SCL;				         // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_10);						 // 输出高
}		

void SCCB_W_SCL(uint8_t BitValue)                        // 写 SCL
{
    GPIO_WriteBit(GPIO_PORT, SCL, (BitAction)BitValue);
    Delay_us(10);
}

void SCCB_W_SDA(uint8_t BitValue)                        // 写 SDA
{
    GPIO_WriteBit(GPIO_PORT, SDA, (BitAction)BitValue);
    Delay_us(10);
}

uint8_t SCCB_R_SDA(void)                                 // 读 SDA                              
{
    Delay_us(10);
    return GPIO_ReadInputDataBit(GPIO_PORT, SDA);
}

void SCCB_Start(void)                                     // SCCB 起始信号
{
    SCCB_W_SDA(1);    //数据线与时钟线初始都为高电平
    Delay_us(10);
    SCCB_W_SCL(1);
    Delay_us(10);
    SCCB_W_SDA(0);    //在时钟线高电平时拉低数据线，再拉低时钟线，表示开始
    Delay_us(10);
    SCCB_W_SCL(0);
    Delay_us(10);
}

void SCCB_Stop(void)
{
    SCCB_W_SDA(0);   //数据线与时钟线初始都为低电平
    Delay_us(10);
    SCCB_W_SCL(1);   //在时钟线高电平时拉高数据线，再拉高时钟线，表示结束
    Delay_us(10);
    SCCB_W_SDA(1);
    Delay_us(10);
}

void SCCB_SendByte(uint8_t SendByte)         //SCCB发送一个字节
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        Delay_us(2);
        SCCB_W_SDA(SendByte & (0x80 >> i));
        Delay_us(2);
        SCCB_W_SCL(1);
        Delay_us(2);
        SCCB_W_SCL(0);
        Delay_us(2);
    }
}

uint8_t SCCB_ReceiveByte(void)                //SCCB接收一个字节
{
    uint8_t i, ReceiveByte = 0;
    SCCB_W_SDA(1);
    for(i = 0; i < 8; i++)
    {
        SCCB_W_SCL(1);
        if(SCCB_R_SDA() == 1){ReceiveByte |= (0x80 >> i);}
        SCCB_W_SCL(0);
        Delay_us(1);
    }
    return ReceiveByte;
}

void SCCB_SendAck(void)                        //发送应答
{
    SCCB_W_SDA(1);
    SCCB_W_SCL(1);
    Delay_us(10);
    SCCB_W_SCL(0);
    Delay_us(10);
    SCCB_W_SDA(0);
}

uint8_t SCCB_ReceiveAck(void)                   //接收应答
{
    uint8_t Ack;
    SCCB_W_SDA(1);
    SCCB_W_SCL(1);
    Ack = SCCB_R_SDA();
    SCCB_W_SCL(0);
    return Ack;
}

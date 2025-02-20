/**
*****************************************************************************
*
*  @file    I2C_Software.c
*  @brief   本文件的函数可完成以下功能：
*			1.软件I2C的初始化配置
*           2.I2C的通信
*
*  @author  binbin
*  @date    2024.08.16
*  @version 1.0
*  @note    可以根据需要修改引脚
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void I2C_Software_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);
}

void I2C_Software_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t I2C_Software_R_SDA(void)
{
	uint8_t BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}

void I2C_Software_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void I2C_Software_Start(void)            //开始逻辑
{
	I2C_Software_W_SCL(1);
	I2C_Software_W_SDA(1);
	I2C_Software_W_SDA(0);           //先拉低SDA
	I2C_Software_W_SCL(0);           //后拉低SCL
}

void I2C_Software_Stop(void)
{
	I2C_Software_W_SDA(0);			  //拉低SDA确保为低电平
	I2C_Software_W_SCL(1);	          //先拉高SCL
	I2C_Software_W_SDA(1);			  //后拉高SDA
}

void I2C_Software_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		I2C_Software_W_SDA(Byte & (0x80 >> i));	//依次存放从高到低每一位
		I2C_Software_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
		I2C_Software_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
	}
}

uint8_t I2C_Software_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	I2C_Software_W_SDA(1);														//主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)											//循环8次，主机依次接收数据的每一位
	{
		I2C_Software_W_SCL(1);													//释放SCL，主机机在SCL高电平期间读取SDA
		if (I2C_Software_R_SDA() == 1){Byte |= (0x80 >> i);} I2C_Software_W_SCL(0);	//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;						
}

void I2C_Software_SendAck(uint8_t AckBit)
{
	I2C_Software_W_SDA(AckBit);					//主机把应答位数据放到SDA线
	I2C_Software_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
	I2C_Software_W_SCL(0);							//拉低SCL，开始下一个时序模块
}

uint8_t I2C_Software_ReceiveAck(void)
{
	uint8_t AckBit;							//定义应答位变量
	I2C_Software_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	I2C_Software_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = I2C_Software_R_SDA();					//将应答位存储到变量里
	I2C_Software_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}



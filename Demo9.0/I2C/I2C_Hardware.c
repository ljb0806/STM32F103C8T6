/**
*****************************************************************************
*
*  @file    I2C_Hardware.c
*  @brief   本文件的函数可完成以下功能：
*			1.I2C2的硬件初始化
*           2.I2C2的字节接收发送
*
*  @author  binbin
*  @date    2024.08.16
*  @version 1.0
*  @note    引脚不可修改！！！
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header

void I2C_Hardware_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS);	//循环等待指定事件
}

void I2C_Hardware_Init(void)
{
//	MyI2C_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
}

void I2C_Hardware_WriteReg(uint8_t SAddress, uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);										    //硬件I2C生成起始条件
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, SAddress, I2C_Direction_Transmitter);	            //硬件I2C发送从机地址，方向为发送
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											    //硬件I2C发送寄存器地址
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//等待EV8
	
	I2C_SendData(I2C2, Data);												    //硬件I2C发送数据
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);		    //等待EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);											    //硬件I2C生成终止条件
}

uint8_t I2C_Hardware_ReadReg(uint8_t SAddress, uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);										    //硬件I2C生成起始条件
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, SAddress, I2C_Direction_Transmitter);	            //硬件I2C发送从机地址，方向为发送
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											    //硬件I2C发送寄存器地址
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);			//等待EV8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);											//硬件I2C生成重复起始条件
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, SAddress, I2C_Direction_Receiver);				//硬件I2C发送从机地址，方向为接收
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		//等待EV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);										//在接收最后一个字节之前提前将应答失能
	I2C_GenerateSTOP(I2C2, ENABLE);												//在接收最后一个字节之前提前申请停止条件
	
	I2C_Hardware_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//等待EV7
	Data = I2C_ReceiveData(I2C2);												//接收数据寄存器
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);										//将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
	
	return Data;
}

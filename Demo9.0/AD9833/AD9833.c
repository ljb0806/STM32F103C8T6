/**
*****************************************************************************
*
*  @file    AD9833.c
*  @brief   本文件的函数可完成以下功能：
*			1.AD9833的初始化
*           2.AD9833的波形设置
*           3.AD9833的幅度设置
*
*  @author  binbin
*  @date    2024.10.09
*  @version 1.0
*  @note    None
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header    
#include "delay.h"
#include "AD9833.h"

void AD9833_Init(void)      //初始化四个管脚
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    FSYNC_1();      //初始化为高电平
    CS_1();         //初始化为高电平
}

void AD9833_WriteReg(uint16_t Data)
{
    SCK_0();           //在时钟上升沿操作
    SCK_1();
    FSYNC_1();         //拉低片选
    FSYNC_0();
    for(uint8_t i = 0; i < 16; i++)
    {
        if(Data&0x8000) DAT_1();    //从高位开始传输
        else DAT_0();

        SCK_0();
        SCK_1();

        Data <<= 1;       //左移一位
    }

    FSYNC_1();          //拉高片选
}

void AD9833_AmpSet(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;
   	
	CS_0();
	temp =0x1100|amp;
	for(i=0;i<16;i++)
	{
	    SCK_0();	
	   if(temp&0x8000)
	   	DAT_1();
	   else
		DAT_0();
		temp<<=1;
	    SCK_1();
	   Delay_us(2);
	}
	
   	CS_1();
}

void AD9833_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************计算频率的16进制值***********************************/
		frequence_mid=268435456/25;						//适合25M晶振
		//如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  					//这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
		frequence_LSB=frequence_hex; 					//frequence_hex低16位送给frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;				//去除最高两位，16位数换去掉高位后变成了14位
		frequence_MSB=frequence_hex>>14;    			//frequence_hex高16位送给frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;				//去除最高两位，16位数换去掉高位后变成了14位

		Phs_data=Phase|0xC000;							//相位值
		AD9833_WriteReg(0x0100); 						//复位AD9833,即RESET位为1
		AD9833_WriteReg(0x2100); 						//选择数据一次写入，B28位和RESET位为1

		if(Freq_SFR==0)				  					//把数据设置到设置频率寄存器0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			//使用频率寄存器0输出波形
			AD9833_WriteReg(frequence_LSB); 			//L14，选择频率寄存器0的低14位数据输入
			AD9833_WriteReg(frequence_MSB); 			//H14 频率寄存器的高14位数据输入
			AD9833_WriteReg(Phs_data);					//设置相位
			//AD9833_Write(0x2000); /**设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形**/
	    }
		if(Freq_SFR==1)									//把数据设置到设置频率寄存器1
		{
			frequence_LSB=frequence_LSB|0x8000;
			frequence_MSB=frequence_MSB|0x8000;
			//使用频率寄存器1输出波形
			AD9833_WriteReg(frequence_LSB); 			//L14，选择频率寄存器1的低14位输入
			AD9833_WriteReg(frequence_MSB); 			//H14 频率寄存器1为
			AD9833_WriteReg(Phs_data);					//设置相位
			//AD9833_Write(0x2800); /**设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形**/
		}

		if(WaveMode==TRI_WAVE) 							//输出三角波波形
		 	AD9833_WriteReg(0x2002); 
		if(WaveMode==SQU_WAVE)							//输出方波波形
			AD9833_WriteReg(0x2028); 
		if(WaveMode==SIN_WAVE)							//输出正弦波形
			AD9833_WriteReg(0x2000); 
}

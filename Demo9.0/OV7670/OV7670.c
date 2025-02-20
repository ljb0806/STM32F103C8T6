/**
*****************************************************************************
*
*  @file    OV7670.c
*  @brief   本文件的函数可完成以下功能：
*			1.OV7670的初始化
*           2.OV7670的读寄存器
*           3.OV7670的写寄存器
*           4.OV7670的特效设置
*           5.OV7670的对比度设置
*           6.OV7670的亮度设置
*           7.OV7670的饱和度设置
*           8.OV7670的亮度模式设置
*           9.OV7670的窗口设置
*           10.OV7670的特效设置
*
*  @author  binbin
*  @date    2024.08.16
*  @version 1.0
*  @note    带 FIFO
*****************************************************************************
**/

#include "stm32f10x.h"
#include "OV7670.h"
#include "OV7670_RegConfig.h"
#include "Delay.h"
#include "SCCB.h"

uint8_t OV7670_ReadReg(uint8_t reg)
{
    uint8_t data;
    SCCB_Start();
    SCCB_SendByte(OV7670_ADDRESS);
    Delay_us(100);
    SCCB_SendByte(reg);
    Delay_us(100);
    SCCB_Stop();
    Delay_us(100);
    SCCB_Start();
    SCCB_SendByte(OV7670_ADDRESS | 0x01);
    Delay_us(100);
    data = SCCB_ReceiveByte();
    SCCB_SendAck();
    SCCB_Stop();

    return data;
}

void OV7670_WriteReg(uint8_t reg, uint8_t data)
{
    SCCB_Start();
    SCCB_SendByte(OV7670_ADDRESS);
    Delay_us(100);
    SCCB_SendByte(reg);
    Delay_us(100);
    SCCB_SendByte(data);
    SCCB_Stop();
}

void OV7670_Init(void)
{
//    uint8_t temp;
    uint16_t i = 0;

    GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 //使能相关端口时钟
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8; 				 //PA8 - VSY 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
		
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 // PB12 - WR
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 // PA11 - RCK
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_11);	
	
	GPIO_InitStructure.GPIO_Pin  = 0xff; //PA0~7 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;  // PA15 - OE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  // PB3 - RRST PB4 - WRST
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD

    SCCB_Init();

    OV7670_WriteReg(0x12, 0x80); // Reset the camera
    Delay_ms(50);
    
    for(i = 0; i < sizeof(ov7670_init_reg_tbl) / sizeof(ov7670_init_reg_tbl[0]); i++)
    {
        OV7670_WriteReg(ov7670_init_reg_tbl[i][0], ov7670_init_reg_tbl[i][1]);
    }
}

void OV7670_Light_Mode(uint8_t mode)
{
    uint8_t reg13val = 0xe7;
    uint8_t reg01val = 0x00;
    uint8_t reg02val = 0x00;

    switch (mode)
    {
        case 1:   //sunny
            reg13val = 0xe7;
            reg01val = 0x5a;
            reg02val = 0x5c;
            break;

        case 2:   //cloudy
            reg13val = 0xe5;
            reg01val = 0x58;
            reg02val = 0x60;
            break;

        case 3:   //office
            reg13val = 0xe5;
            reg01val = 0x84;
            reg02val = 0x4c;
            break;

        case 4:   //home
            reg13val = 0xe5;
            reg01val = 0x96;
            reg02val = 0x40;
            break;    
    }

    OV7670_WriteReg(0x13, reg13val);
    OV7670_WriteReg(0x01, reg01val);
    OV7670_WriteReg(0x02, reg02val);
}

void OV7670_Color_Saturation(uint8_t sat)
{
    uint8_t reg4f5054val = 0x80; //sat = 2
    uint8_t reg52val = 0x22;
    uint8_t reg53val = 0x5e;

    switch (sat)
    {
        case 1:
            reg4f5054val = 0x40;
            reg52val = 0x11;
            reg53val = 0x2f;
            break;

        case 2:
            reg4f5054val = 0x66;
            reg52val = 0x1b;
            reg53val = 0x4b;
            break;

        case 3:
            reg4f5054val = 0x99;
            reg52val = 0x28;
            reg53val = 0x71;
            break;

        case 4:
            reg4f5054val = 0xc0;
            reg52val = 0x33;
            reg53val = 0x8d;
            break;
    }

    OV7670_WriteReg(0x4f, reg4f5054val);
    OV7670_WriteReg(0x50, reg4f5054val);
    OV7670_WriteReg(0x54, reg4f5054val);
    OV7670_WriteReg(0x52, reg52val);
    OV7670_WriteReg(0x53, reg53val);
    OV7670_WriteReg(0x51, 0x00);
    OV7670_WriteReg(0x58, 0x9e);
}

void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	OV7670_WriteReg(0X55,reg55val);	//亮度调节 
}

void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//默认就是contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	OV7670_WriteReg(0X56,reg56val);	//对比度调节 
}

void OV7670_Special_Effects(u8 eft)




{
	u8 reg3aval=0X04;//默认为普通模式
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	OV7670_WriteReg(0X3A,reg3aval);//TSLB设置 
	OV7670_WriteReg(0X68,reg67val);//MANU,手动U值 
	OV7670_WriteReg(0X67,reg68val);//MANV,手动V值 
}	

void OV7670_Window_Set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t endx = sx + width * 2;
    uint16_t endy = sy + height * 2;
    uint8_t temp;

    if(endy > 784) endy -= 784;
    temp = OV7670_ReadReg(0x03);
    temp &= 0xf0;
    temp |= ((endx & 0x03) << 2) | (sx & 0x03);
    OV7670_WriteReg(0x03, temp);
    OV7670_WriteReg(0x19, sx >> 2);
    OV7670_WriteReg(0x1a, endx >> 2);

    temp = OV7670_ReadReg(0x32);
    temp &= 0xc0;
    temp |= (endy & 0x07) << 3 | (sy & 0x07);
    OV7670_WriteReg(0x17, sy >> 3);
    OV7670_WriteReg(0x18, endy >> 3);
}

void OV7670_Effects_Set(void)
{
    uint8_t lightmode = 0, effect = 0;
    uint8_t contrast = 0, brightness = 0, saturation = 4;

    OV7670_Light_Mode(lightmode);
    OV7670_Color_Saturation(saturation);
    OV7670_Brightness(brightness);
    OV7670_Contrast(contrast);
    OV7670_Special_Effects(effect);

    OV7670_Window_Set(12, 176, 240, 320);
}

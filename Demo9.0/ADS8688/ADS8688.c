#include "stm32f10x.h"
#include "ADS8688.h"
#include "Delay.h"

void ADS8688_GPIO_Init(void)
{ 
	GPIO_InitTypeDef GPIO_Initure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_7; 
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式  
	GPIO_Initure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initure); 
	GPIO_Initure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_3; 
	GPIO_Init(GPIOA,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_5; 
	GPIO_Init(GPIOA,&GPIO_Initure);
	GPIO_Initure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Initure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_Initure);
}

void ADS8688_SPI_WB(uint8_t data)
{
    uint8_t temp = data;                  // 中间变量
    ADS_8688_nCS_L;                       // 拉低片选信号

    for(uint8_t i = 0; i < 8; i++)
    {
        if(temp & 0x80) {ADS_8688_SDI_H;}   // 最高位为1则拉高数据线
        else            {ADS_8688_SDI_L;}   // 最高位为0则拉低数据线

        ADS_8688_SCLK_H;                  // 上升沿时钟
        temp <<= 1;                       // 数据左移一位
        ADS_8688_SCLK_L;                  // 下降沿时钟
    }
}

uint8_t ADS8688_SPI_RB(void)
{
    uint8_t data = 0;
    ADS_8688_nCS_L;                       // 拉低片选信号

    for(uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;                       // 数据左移一位
        ADS_8688_SCLK_H;                  // 上升沿时钟

        if(ADS_8688_SDO) {data |= 0x01;}  // 读取数据线
        else             {data &= 0xFE;}       

        ADS_8688_SCLK_L;                  // 下降沿时钟0
    }

    return data;
}

void ADS8688_WriteCommandReg(uint16_t command)
{
    ADS_8688_nCS_L;                       // 拉低片选信号
    ADS8688_SPI_WB(command >> 8 & 0xFF);  // 写入高8位
    ADS8688_SPI_WB(command & 0xFF);       // 写入低8位
    ADS_8688_nCS_H;                       // 拉高片选信号
}

void ADS8688_Write_Program_Register(uint8_t Addr, uint8_t data)
{
    ADS_8688_nCS_L;                                   // 拉低片选信号
    ADS8688_SPI_WB(Addr << 1 | 0x01);          // 写入地址
    ADS8688_SPI_WB(data);                      // 写入数据
    ADS_8688_nCS_H;                                   // 拉高片选信号
}

uint8_t ADS8688_READ_Program_Register(uint8_t Addr)
{
    uint8_t data = 0;
    ADS_8688_nCS_L;                                   // 拉低片选信号
    
    ADS8688_SPI_WB(Addr << 1);                 // 写入地址
    data = ADS8688_SPI_RB();                    // 读取数据
    data = ADS8688_SPI_RB();                    
    ADS_8688_nCS_H;                                   // 拉高片选信号

    return data;
}

void Enter_RESET_MODE(void)
{
    ADS8688_WriteCommandReg(RST);                     // 写入复位命令
}

void AUTO_RST_MODE(void)
{
    ADS8688_WriteCommandReg(AUTO_RST);                // 自动扫描模式
}

void MAN_Ch_n_Mode(uint16_t ch)
{
    ADS8688_WriteCommandReg(ch);                      // 手动模式
}

void Set_CH_Range_Select(uint8_t ch, uint8_t range)
{
    ADS8688_Write_Program_Register(ch, range);        // 写入通道范围选择
}

void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum)
{
    uint8_t datah = 0, datal = 0;
    uint16_t data = 0;
    for(uint8_t i = 0; i < chnum; i++)
    {
        ADS_8688_nCS_L;                                // 拉低片选信号
        ADS8688_SPI_WB(0x00);                   // 写入数据
        ADS8688_SPI_WB(0x00);                   // 写入数据
        datah = ADS8688_SPI_RB();                // 读取数据
        datal = ADS8688_SPI_RB();                // 读取数据
        ADS_8688_nCS_H;                                // 拉高片选信号
        data = datah << 8 | datal;                     // 数据合并
        *(outputdata + i) = data;                      // 保存数据
    }
}

uint16_t Get_MAN_ch_n_Mode_Data(void)
{
    uint8_t datah = 0, datal = 0;
    ADS_8688_nCS_L;                                   // 拉低片选信号
    ADS8688_SPI_WB(0x00);                             // 写入数据
    ADS8688_SPI_WB(0x00);                             // 写入数据
    datah = ADS8688_SPI_RB();                         // 读取数据
    datal = ADS8688_SPI_RB();                         // 读取数据
    ADS_8688_nCS_H;                                   // 拉高片选信号
    return datah << 8 | datal;                        // 数据合并
}

void ADS8688_Init_Mult(void) 
{ 
    ADS8688_GPIO_Init();
    ADS_8688_RST_PD_H; 
	ADS_8688_DAISY_IN_L; 
	Delay_us(10);
	Enter_RESET_MODE();
	//进入配置
	ADS8688_Write_Program_Register(0X00,0X00);
	Delay_us(10);
	//开启通道
	ADS8688_Write_Program_Register(0X01,0X07);
	Delay_us(10);
	//下拉
	ADS8688_Write_Program_Register(0x02,0xf8);//所有通道退出低功耗状态 
	Delay_us(10);
	//设置功能
	ADS8688_Write_Program_Register(0x03,0x03);
	Delay_us(10);
	//ADS8688_Write_Program_Register(0x01,0xff);//使能所有通道 
	Set_CH_Range_Select(CH1,0x00);//设置通道1的输入范围：+-2.5*Vref 
	Delay_us(10);
	
	Set_CH_Range_Select(CH2,0x00); 
	Delay_us(10);
	
	Set_CH_Range_Select(CH3,0x00);
	Delay_us(10);
	/*
	Set_CH_Range_Select(CH2,0x00);
	Set_CH_Range_Select(CH3,0x00);
	Set_CH_Range_Select(CH4,0x00); 
	Set_CH_Range_Select(CH5,0x00); 
	Set_CH_Range_Select(CH6,0x00); 
	Set_CH_Range_Select(CH7,0x00); 
	
	*/
	// //0x00 -> +-2. 5*ref 
	// //0x01 -> +-1.25*ref 
	// //0x02 -> +-0.625*ref 
	// //0x03 -> +2.5*ref 
	// //0x04 -> +1.25*ref 
	//AUTO_RST_Mode();//进入自动扫描模式 
	ADS8688_Write_Program_Register(0xa0,0x03);
	Delay_us(10);
    AUTO_RST_MODE();
}


u16 get_ADS_ch1(void) 
{ 
	MAN_Ch_n_Mode(MAN_Ch_1);//需要手动采集哪个通道就将函数的变量改为那个通道
	return Get_MAN_ch_n_Mode_Data();//读取通道1数据,具体通道数由函数 MAN_Ch_n_Mode()所决定
}

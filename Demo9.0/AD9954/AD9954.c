#include "stm32f10x.h"
#include "AD9954.h"
#include "Delay.h"

#define fosc  20                        //晶振频率 20Mhz
#define PLL_MULTIPLIER  20              //PLL倍频数（4--20）
#define fs (fosc*PLL_MULTIPLIER)        //系统时钟频率

double fH_Num=10.73741824;

/* 管口初始化 */
void AD9954_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 全部置0 */
	AD9954_RESET = 0;
	AD9954_CS  = 0;
	AD9954_SCLK = 0;
	AD9954_SDIO = 0;
	AD9954_OSK = 0;
	AD9954_PS0 = 0;
	AD9954_PS1 = 0;
	AD9954_IOUPDATE = 0;
	AD9954_IOSY = 0;
	AD9954_PWR = 0;

	Delay_ms(10);
}

/* 复位 */
void AD9954_RST(void)
{
    AD9954_RESET = 1;
    Delay_ms(10);
    AD9954_RESET = 0;
    AD9954_CS = 0;
    AD9954_SCLK = 0;
    AD9954_CS = 1;
}

/* 更新寄存器 */
void AD9954_UPDATE(void)
{
    AD9954_IOUPDATE = 1;
    Delay_us(10);
    AD9954_IOUPDATE = 0;
}

/* 发送字节 */
void AD9954_SendByte(uint8_t data)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		AD9954_SCLK = 0;
		Delay_us(10);

		if (data & 0x80) AD9954_SDIO = 1;
		else AD9954_SDIO = 0;
		
		AD9954_SCLK = 1;
		Delay_us(10);

		data <<= 1;
	}
}

/* 接收字节 */
uint8_t AD9954_ReadByte(void)
{
	uint8_t data = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		AD9954_SCLK = 0;
		Delay_us(2);

		data |= AD9954_SDO;

		AD9954_SCLK = 1;
		Delay_us(2);

		data <<= 1;
	}
	return data;
}

/* 发送多个字节 */
void AD9954_SendBytes(uint8_t RegAddr, uint8_t *Data, uint8_t Len)
{
    AD9954_SendByte(RegAddr);

    for(uint8_t i = 0; i < Len; i++)
    {
        AD9954_SendByte(Data[i]);
    }
}

/* 读取多个字节 */
uint32_t AD9954_ReadBytes(uint8_t RegAddr, uint8_t Len)
{
    uint32_t temp;
    AD9954_CS = 0;
    AD9954_SendByte(RegAddr);

    for(uint8_t i = 0; i < Len; i++)
    {
        temp <<= 8;
        temp += AD9954_ReadByte();
    }

    AD9954_CS = 1;
    return temp;

}

/* 获取需要频率 */
uint32_t Get_FTW(double Real_fH)
{
    return (uint32_t)(fH_Num * Real_fH);
}

/* AD9954初始化 */
void AD9954_Init(void)
{
    uint8_t CFR1_data[4] = {0, 0, 0, 0};
    uint8_t CFR2_data[3] = {0, 0, 0};

    AD9954_GPIO_Init();
    AD9954_RST();
    Delay_ms(300);
    AD9954_CS = 0;

    CFR1_data[0]=0X02; // 此处：0x02->OSK使能；0X00->OSK关闭。在OSK模式使能的前提下，幅度寄存器(0X02)生效
    CFR1_data[1]=0X00;
	CFR1_data[2]=0X00;
	CFR1_data[3]=0x00; // 比较器启用,方波输出;0x40,比较器禁用方波无输出
    AD9954_SendBytes(CFR1, CFR1_data, 4);

    CFR2_data[0]=0X00;
    CFR2_data[1]=0X00;
    if(fs > 400)                                                       ;                               
    else if(fs >= 250) CFR2_data[2] = PLL_MULTIPLIER << 3 | 0x04 | 0x03;
    else               CFR2_data[2] = PLL_MULTIPLIER << 3;
    AD9954_SendBytes(CFR2, CFR2_data, 3);

    AD9954_CS = 1;
}

/* 设置频率 */
void AD9954_Set_Fre(double fre)
{
    uint8_t data[4] = {0x00, 0x00, 0x00, 0x00};
    uint32_t temp = Get_FTW(fre);

    data[0] = (uint8_t)(temp >> 24);
    data[1] = (uint8_t)(temp >> 16);
    data[2] = (uint8_t)(temp >> 8);
    data[3] = (uint8_t)(temp);

    AD9954_CS = 0;
    AD9954_SendBytes(FTW0, data, 4);
    AD9954_CS = 1;
    AD9954_UPDATE();
}

/* 设置幅度 */
void AD9954_Set_Amp(uint16_t amp)
{
    uint8_t data[2] = {0x00, 0x00};
    AD9954_CS = 0;

    data[0] = (uint8_t)(amp >> 8);
    data[1] = (uint8_t)(amp);

    AD9954_SendBytes(ASF, data, 2);
    AD9954_CS = 1;

    AD9954_UPDATE();
}

/* 设置相位 */
void AD9954_Set_Phase(uint16_t phase)
{
    uint8_t data[2] = {0x00, 0x00};
    AD9954_CS = 0;

    data[0] = (uint8_t)(phase >> 8);
    data[1] = (uint8_t)(phase);

    AD9954_SendBytes(POW0, data, 2);
    AD9954_CS = 1;

    AD9954_UPDATE();
}






























































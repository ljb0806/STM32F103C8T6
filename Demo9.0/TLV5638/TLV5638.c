#include "stm32f10x.h"
#include "Delay.h"
#include "TLV5638.h"
#include "GPIOS.h"

/* 管脚电平控制 */
#define SCLK_0 GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define SCLK_1 GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define DIN_0 GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define DIN_1 GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define CS_0 GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define CS_1 GPIO_SetBits(GPIOA, GPIO_Pin_5)

/* GPIO初始化 */
void TLV5638_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;   // 对应 SCLK, DIN, CS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    CS_1; 
    DIN_0; 
    SCLK_0;
}

/* 发送16字节数据 */
void TLV5638_Send2Byte(uint16_t data)
{
    CS_0;
	uint16_t temp = data;
    for(uint8_t i = 0; i < 16; i++)
    {
        if(temp & 0x8000) DIN_1;
        else DIN_0;
        
        SCLK_1;
        Delay_us(1);
        SCLK_0;
        Delay_us(1);

		temp <<= 1;
    }

    CS_1;
}

/* 设置DACB输出 */
void TLV5638_Set_DACB(uint16_t DAC_Value)
{
    TLV5638_Send2Byte(LOAD_B_BUFFER | DAC_Value);
}

/* 设置DACA输出 */ 
void TLV5638_Set_DACA(uint16_t DAC_Value)
{
    TLV5638_Send2Byte(LOAD_A_UPDATE_BUFFER_B | DAC_Value);
}

/* 关机 */
void TLV5638_PWR_DOWN(void)
{
    TLV5638_Send2Byte(PWR_DOWN);
}

/* 参考电压1.024V */
void TLV5638_Set_Vref_1024(void)
{
    TLV5638_Send2Byte(VREF1024);
}

/* 参考电压2.048V */
void TLV5638_Set_Vref_2048(void)
{
    TLV5638_Send2Byte(VREF2048);
}

/* 完整初始化 */
void TLV5638_Init(uint16_t VREF)
{
    TLV5638_GPIO_Init();
    if(VREF == VREF1024)      TLV5638_Set_Vref_1024();
    else if(VREF == VREF1024) TLV5638_Set_Vref_2048();
}























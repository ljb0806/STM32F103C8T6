/**
*****************************************************************************
*
*  @file    GPIOS.c
*  @brief   本文件的函数可完成以下功能：
*			1.各种GPIO的初始化
*           2.设置GPIO的高低电平
*
*  @author  binbin
*  @date    2024.10.09
*  @version 2.0
*  
*****************************************************************************
**/

#include "stm32f10x.h"
#include "GPIOS.h"

void Config_GPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_Mode_TypeDef Mode) 
{
    // 使能 GPIO 时钟
    if (GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } else if (GPIOx == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    } else if (GPIOx == GPIOE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    }

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if (Mode == GPIO_MODE_OUTPUT) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    } else {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void Set_GPIO_High(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_SetBits(GPIOx, GPIO_Pin);
}

void Set_GPIO_Low(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_ResetBits(GPIOx, GPIO_Pin);
}


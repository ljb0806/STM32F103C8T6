/**
*****************************************************************************
*
*  @file    ADC_1.c
*  @brief   本文件的函数可完成以下功能：
*			1.ADC1通道1的初始化
*           2.读取ADC1通道1的转化值
*
*  @author  binbin
*  @date    2024.08.15
*  @version 1.0
*  @note    通道1输入引脚为PA0，注意不要引脚冲突，时钟分频后必须保证不超过14MHz
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header

void ADC1_Init(void)
{
	//开启总线时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//给ADC时钟分频
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);             //72M / 6 = 12M      
	
	//GPIO初始化为ADC输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ADC输入的专属IO口模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//配置ADC1通道0的序列号为1，采样周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);     //ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;                                     //结构体
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                     //独立转化模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                 //数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //外部触发方式选择无，使用内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                    //单次转换模式（每次转化需要手动触发）
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                          //非扫描模式（多通道才需要扫描模式）
	ADC_InitStructure.ADC_NbrOfChannel = 1;                                //通道数目
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);                       //复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//等待复位校准
	ADC_StartCalibration(ADC1);                       //开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);     //等待校准完成
}

uint16_t ADC1_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);             		  //开始一次转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);     //等待转换完成
	return ADC_GetConversionValue(ADC1);
}	

/**
*****************************************************************************
*
*  @file    ADC_2.c
*  @brief   ���ļ��ĺ�����������¹��ܣ�
*			1.ADC2ͨ��1�ĳ�ʼ��
*           2.��ȡADC2ͨ��1��ת��ֵ
*
*  @author  binbin
*  @date    2024.08.15
*  @version 1.0
*  @note    ͨ��1��������ΪPA0��ע�ⲻҪ���ų�ͻ��ʱ�ӷ�Ƶ����뱣֤������14MHz
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header

void ADC2_Init(void)
{
	//��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//��ADCʱ�ӷ�Ƶ
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);             //72M / 6 = 12M      
	
	//GPIO��ʼ��ΪADC����ģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ADC�����ר��IO��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//����ADC2ͨ��0�����к�Ϊ1����������
	ADC_RegularChannelConfig(ADC2,ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;                                     //�ṹ��
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                     //����ת��ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                 //�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //�ⲿ������ʽѡ���ޣ�ʹ���ڲ���������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                    //����ת��ģʽ��ÿ��ת����Ҫ�ֶ�������
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                          //��ɨ��ģʽ����ͨ������Ҫɨ��ģʽ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;                                //ͨ����Ŀ
	ADC_Init(ADC2,&ADC_InitStructure);
	
	ADC_Cmd(ADC2,ENABLE);
	
	ADC_ResetCalibration(ADC2);                       //��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC2) == SET);//�ȴ���λУ׼
	ADC_StartCalibration(ADC2);                       //��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC2) == SET);     //�ȴ�У׼���
}

uint16_t ADC2_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);             		  //��ʼһ��ת��
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);     //�ȴ�ת�����
	return ADC_GetConversionValue(ADC2);
}	

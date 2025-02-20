/**
*****************************************************************************
*
*  @file    Timer2.c
*  @brief   本文件的函数可完成以下功能：
*			1.TIM2初始化
*           2.定时进中断
*
*  @author  binbin
*  @date    2024.08.15
*  @version 1.0
*  @note    定时时间可以自己修改，本例程不修改的话是72M / 10000 / 7200 = 1s，每1s进一次中断
*****************************************************************************
**/

#include "stm32f10x.h"                  // Device header

void Timer2_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);                    //TIM2在总线APB1上，因此打开APB1总线上的时钟
	TIM_InternalClockConfig(TIM2);                                         //TIM2配置为内部72M时钟（不调用该函数也可以，默认也是使用内部时钟）
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;                       //先定义时基单元结构体
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;               //滤波分频（没有多大用）
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;           //加法计数
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;                         //计数周期
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;                       //时钟分频
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;                      //重复计数功能，通用计数器没有，置0
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);                        //将结构体初始化
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                               //使能中断为更新中断
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                                   //清掉中断标志位，防止开始就进中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                        //配置NVIC分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                        //选择TIM2中断通道 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //打开通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;              //抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                     //响应优先级为1
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM2,ENABLE);                                                  //启动！
}

void TIM2_IRQHandler(void)                                                 //TIM2的中断函数，你可以移到别的地方使用
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                        //清掉中断标志位，不然再也进不了中断了
	}
}

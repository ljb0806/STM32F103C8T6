#ifndef __TLV5638_H_
#define __TLV5638_H_

#include "stm32f10x.h"

// TLV5638的寄存器为16位 使用16位SPI通信
// 前四位为配置寄存器
// 后12位为数据寄存器

/* 前四位寄存器配置 */

// 1.直接配置的
#define PWR_DOWN 0x2000                // 关机
#define VREF1024 0xD001                // 参考电压1.024V
#define VREF2048 0xD002                // 参考电压2.048V

// 2.配合函数调用修改数值的
#define LOAD_B_BUFFER           0x4000           // 数据位写入DACB与缓存
#define LOAD_BUFFER             0x5000           // 数据位写入缓存
#define LOAD_A_UPDATE_BUFFER_B  0xC000           // 数据位写入A，缓存写入B


/* 函数封装 */
void TLV5638_Init(uint16_t VREF);
void TLV5638_Set_DACA(uint16_t DAC_Value);
void TLV5638_Set_DACB(uint16_t DAC_Value);
void TLV5638_PWR_DOWN(void);
#endif

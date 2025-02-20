#ifndef __AD9954_H
#define __AD9954_H

/* 头文件 */
#include "stm32f10x.h"
#include "GPIO.h"

/* 引脚 */
#define AD9954_CS       PBout(9)
#define AD9954_SCLK     PBout(10)
#define AD9954_SDIO     PBout(11)
#define AD9954_OSK      PBout(12)
#define AD9954_PS1      PBout(13)
#define AD9954_PS0      PBout(14)
#define AD9954_IOUPDATE PBout(15)
#define AD9954_RESET    PBout(7)
#define AD9954_PWR      PBout(8)
#define AD9954_IOSY     PBout(6)
#define AD9954_SDO      PBin(5)

#define CFR1	0X00	// 控制功能寄存器1
#define CFR2    0X01	// 控制功能寄存器2
#define ASF     0X02	// 振幅比例因子寄存器
#define ARR     0X03	// 振幅斜坡速率寄存器

#define FTW0    0X04	// 频率调谐字寄存器0
#define POW0    0X05	// 相位偏移字寄存器
#define FTW1    0X06	// 频率调谐字寄存器1

#define NLSCW   0X07	// 下降扫描控制字寄存器
#define PLSCW   0X08	// 上升扫描控制字寄存器

#define RSCW0   0X07	
#define RSCW1   0X08	
#define RSCW2   0X09	
#define RSCW3   0X0A	// RAM段控制字寄存器
#define RAM     0X0B	// 读取指令写入RAM签名寄存器数据

#define No_Dwell	0x04	// No_Dwell不停留，输出扫频到终止频率回到起始频率。
#define Dwell 		0x00	// Dwell停留，输出扫频到终止频率后保持在终止频率。

void AD9954_Init(void);
void AD9954_Set_Fre(double fre);
void AD9954_Set_Amp(uint16_t amp);
void AD9954_Set_Phase(uint16_t phase);

#endif

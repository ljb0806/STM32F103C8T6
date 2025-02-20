/* 主函数 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"                      // 延时 s ms us
#include "Serial.h"
#include "TLV5638.h"
#include "AD9833.h"
#include "AD9954.h"

int main(void)
{	
	//TLV5638_Init(VREF1024);
	AD9954_Init();
	AD9954_Set_Fre(10000);
	AD9954_Set_Amp(0x3fff);
	
	
	while(1)
	{
		
	}
}

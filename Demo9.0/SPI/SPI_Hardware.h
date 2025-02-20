#ifndef __SPI_HARDWARE_H
#define __SPI_HARDWARE_H

void SPI_Hardware_Init(void);
void SPI_Hardware_Start(void);
void SPI_Hardware_Stop(void);
uint8_t SPI_Hardware_SwapByte(uint8_t ByteSend);

#endif

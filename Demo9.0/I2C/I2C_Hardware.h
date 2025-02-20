#ifndef __I2C_Hardware_H
#define __I2C_Hardware_h

void I2C_Hardware_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t I2C_Hardware_ReadReg(uint8_t RegAddress);
void I2C_Hardware_Init(void);
void I2C_Hardware_GetData(int16_t *Accx, int16_t *Accy, int16_t *Accz, int16_t *Gyrox, int16_t *Gyroy, int16_t *Gyroz);
uint8_t I2C_Hardware_GetID(void);

#endif

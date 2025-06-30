#ifndef __INT_EEPROM_H__
#define __INT_EEPROM_H__

#include "i2c.h"

#define EEPROM_I2C_ADDRESS 0xA0

void Int_EEPROM_Write_Byte(uint16_t addr, uint8_t data);

uint8_t Int_EEPROM_Read_Byte(uint16_t addr);

#endif /* __INT_EEPROM_H__ */
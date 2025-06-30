#include "Int_EEPROM.h"


/**
 * @brief 写入一个字节到EEPROM指定地址
 *
 * @param addr 要写入的EEPROM地址 (16位)
 * @param data 要写入的数据 (8位)
 */
void Int_EEPROM_Write_Byte(uint16_t addr, uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c2, EEPROM_I2C_ADDRESS, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000); // I2C_MEMADD_SIZE_8BIT为EEPROM内部内存地址位数
    // 等待写入EEPROM完成
    HAL_Delay(5);
}

/**
 * @brief 从EEPROM指定地址读取一个字节
 *
 * @param addr 要读取的EEPROM地址 (16位)
 *
 * @return 读取到的数据 (8位)
 */
uint8_t Int_EEPROM_Read_Byte(uint16_t addr)
{
    uint8_t rx_data = 0;
    HAL_I2C_Mem_Read(&hi2c2, EEPROM_I2C_ADDRESS, addr, I2C_MEMADD_SIZE_8BIT, &rx_data, 1, 1000);
    return rx_data;
}

#include "App_Display.h"

/**
 * 展示页面0:
 *  第一行:    尚硅谷电机项目
 *  第二行:    设置的速度
 *  第三行:    编码器读取计算的速度
 * 展示页面1:
 *     modbus_RS485 从设备的ID
 */
// 马达ID存放地址
extern uint8_t motor_id_addr;
// 显示的页面索引
extern uint8_t page_index;
// 电机ID
extern uint8_t motor_id;
// 电机速度,为正表示正转，为负表示反转
extern int16_t set_motor_speed;
// 电机编码器读取的电机速度,为正表示正转，为负表示反转
extern int16_t measure_motor_speed;

/**
 * @brief 初始化OLED
 *
 */
void App_Display_Init(void)
{
    Int_OLED_Init();
}

void App_Motor_ID_Init(void)
{
    // ID值存储在0x56位置 => 把0x55 => 66
    // 初始化之后,先读取0x55位置的值 判断当前是否是之前存储的校验值
    uint8_t check_value = Int_EEPROM_Read_Byte(0x55);
    if (check_value == 66)
    {
        motor_id = Int_EEPROM_Read_Byte(motor_id_addr);
        Debug_printf("motor_id1:%d\r\n", motor_id);
    }
    // 没有存储过ID,或ID值被覆盖
    else if (check_value != 66)
    {
        Int_EEPROM_Write_Byte(0x55, 66);
        Int_EEPROM_Write_Byte(motor_id_addr, motor_id);
        Debug_printf("motor_id2: %d\n", motor_id);
    }
}

/**
 * @brief 将数据显示在OLED屏幕上
 *
 */
void App_Display_Show(void)
{
    switch (page_index)
    {
        case 0:
            // 显示项目名称
            // for (uint8_t i = 3; i < 7; i++)
            // {
            //     Int_OLED_ShowChinese(16 * (i - 1), 0, i, 16, 1);
            // }
            Int_OLED_ShowString(20, 0, "Motor Speed", 16, 1);
            // 显示设置的电机速度
            Int_OLED_ShowString(0, 16, "Target:", 16, 1);
            if (set_motor_speed >= 0)
            {
                Int_OLED_ShowChar(60, 16, '+', 16, 1);
                Int_OLED_ShowNum(70, 16, set_motor_speed, 4, 16, 1);
            }
            else if (set_motor_speed < 0)
            {
                Int_OLED_ShowString(60, 16, "-", 16, 1);
                Int_OLED_ShowNum(70, 16, -set_motor_speed, 4, 16, 1);
            }
            // 显示实时电机速度
            Int_OLED_ShowString(0, 32, "NOW:", 16, 1);
            if (measure_motor_speed >= 0)
            {
                Int_OLED_ShowString(60, 32, "+", 16, 1);
                Int_OLED_ShowNum(70, 32, measure_motor_speed, 4, 16, 1);
            }
            else if (measure_motor_speed < 0)
            {
                Int_OLED_ShowString(60, 32, "-", 16, 1);
                Int_OLED_ShowNum(70, 32, -measure_motor_speed, 4, 16, 1);
            }
            break;
        case 1:
            Int_OLED_ShowString(0, 16, "ID:", 16, 1);
            Int_OLED_ShowString(66, 16, "^_^", 16, 1);
            Int_OLED_ShowNum(36, 16, motor_id, 3, 16, 1);
            break;
    }
    // 刷新屏幕
    Int_OLED_Refresh();
}
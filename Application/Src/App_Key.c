#include "App_Key.h"

// 按键值
uint8_t key_value = 0;
// 显示页面索引
uint8_t page_index = 0;

/**
 * @brief 处理按键事件，实现页面切换、电机控制和马达ID设置功能
 *
 * 参数:无
 *
 * 返回值:无
 */
void App_Key_Process(void)
{
    // Debug_printf("App_Key_Process");
    //  显示页面切换
    if (key_value == 1)
    {
        // 循环page_index的值在0和1之间
        page_index = (page_index + 1) % 2;
        // 清除显示
        Int_OLED_Clear();
        // 清除按键值
        key_value = 0;
    }
    switch (page_index)
    {
        case 0:
            /**
             * 页面0处理逻辑：电机控制相关操作
             * 包含启动/停止电机、增加/降低电机速度功能
             */
            if (key_value == 2)
            {
                // 启动电机
                if (motor_status == 0)
                {
                    Int_BLDC_Start();
                    Debug_printf("start motor\n");
                }
                else if (motor_status == 1)
                {
                    Int_BLDC_Stop();
                    Debug_printf("stop motor\n");
                }
                // 清除按键值
                key_value = 0;
            }
            // 降低电机速度
            else if (key_value == 3)
            {
                set_motor_speed -= 200;
                if (set_motor_speed < -3500)
                {
                    set_motor_speed = -3500;
                }
                // 清除按键值
                key_value = 0;
            }
            // 增加电机速度
            else if (key_value == 4)
            {
                set_motor_speed += 200;
                if (set_motor_speed > 3500)
                {
                    set_motor_speed = 3500;
                }
                // 清除按键值
                key_value = 0;
            }
            Int_BLDC_Update_Speed(set_motor_speed);
            break;
        case 1:
            /**
             * 页面1处理逻辑：马达ID设置相关操作
             * 包含马达ID递增、递减和写入EEPROM功能
             */
            // 电机ID递增
            if (key_value == 2)
            {
                motor_id++;
                if (motor_id > 8)
                {
                    motor_id = 8;
                }
                // 清除按键值
                key_value = 0;
            }
            // 电机ID递减
            else if (key_value == 3)
            {
                motor_id--;
                if (motor_id < 1)
                {
                    motor_id = 1;
                }
                // 清除按键值
                key_value = 0;
            }
            // 将电机ID写入EEPROM
            Int_EEPROM_Write_Byte(motor_id_addr, motor_id);
            break;
    }
}

/**
 * @brief  外部中断回调函数，用于处理按键事件。
 * @note   此函数在外部中断触发后被调用，用于识别具体哪个按键被按下，
 *         并设置对应的按键值（key_value），以便后续处理逻辑使用。
 * @param  GPIO_Pin: 触发中断的GPIO引脚。
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 检查是否为按键相关的GPIO引脚触发中断
    if (GPIO_Pin == KEY_4_Pin || GPIO_Pin == KEY_3_Pin || GPIO_Pin == KEY_2_Pin || GPIO_Pin == KEY_1_Pin)
    {
        // 延时消抖：防止机械按键因抖动产生误触发
        HAL_Delay(10);

        // 读取按键状态并设置对应按键值
        if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == GPIO_PIN_RESET)
        {
            key_value = 4;
            Debug_printf("key4\n");
        }
        else if (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == GPIO_PIN_RESET)
        {
            key_value = 3;
            Debug_printf("key3\n");
        }
        else if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET)
        {
            key_value = 2;
            Debug_printf("key2\n");
        }
        else if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET)
        {
            key_value = 1;
            Debug_printf("key1\n");
        }
    }
}
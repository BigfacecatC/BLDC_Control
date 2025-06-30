#include "App_Display.h"

/**
 * չʾҳ��0:
 *  ��һ��:    �й�ȵ����Ŀ
 *  �ڶ���:    ���õ��ٶ�
 *  ������:    ��������ȡ������ٶ�
 * չʾҳ��1:
 *     modbus_RS485 ���豸��ID
 */
// ���ID��ŵ�ַ
extern uint8_t motor_id_addr;
// ��ʾ��ҳ������
extern uint8_t page_index;
// ���ID
extern uint8_t motor_id;
// ����ٶ�,Ϊ����ʾ��ת��Ϊ����ʾ��ת
extern int16_t set_motor_speed;
// �����������ȡ�ĵ���ٶ�,Ϊ����ʾ��ת��Ϊ����ʾ��ת
extern int16_t measure_motor_speed;

/**
 * @brief ��ʼ��OLED
 *
 */
void App_Display_Init(void)
{
    Int_OLED_Init();
}

void App_Motor_ID_Init(void)
{
    // IDֵ�洢��0x56λ�� => ��0x55 => 66
    // ��ʼ��֮��,�ȶ�ȡ0x55λ�õ�ֵ �жϵ�ǰ�Ƿ���֮ǰ�洢��У��ֵ
    uint8_t check_value = Int_EEPROM_Read_Byte(0x55);
    if (check_value == 66)
    {
        motor_id = Int_EEPROM_Read_Byte(motor_id_addr);
        Debug_printf("motor_id1:%d\r\n", motor_id);
    }
    // û�д洢��ID,��IDֵ������
    else if (check_value != 66)
    {
        Int_EEPROM_Write_Byte(0x55, 66);
        Int_EEPROM_Write_Byte(motor_id_addr, motor_id);
        Debug_printf("motor_id2: %d\n", motor_id);
    }
}

/**
 * @brief ��������ʾ��OLED��Ļ��
 *
 */
void App_Display_Show(void)
{
    switch (page_index)
    {
        case 0:
            // ��ʾ��Ŀ����
            // for (uint8_t i = 3; i < 7; i++)
            // {
            //     Int_OLED_ShowChinese(16 * (i - 1), 0, i, 16, 1);
            // }
            Int_OLED_ShowString(20, 0, "Motor Speed", 16, 1);
            // ��ʾ���õĵ���ٶ�
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
            // ��ʾʵʱ����ٶ�
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
    // ˢ����Ļ
    Int_OLED_Refresh();
}
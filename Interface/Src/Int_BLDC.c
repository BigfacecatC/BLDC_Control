#include "Int_BLDC.h"

// 电机ID存放地址
uint8_t motor_id_addr = 0x56;
// 电机当前状态 0 停止 1 启动
uint8_t motor_status = 0;
// 1:正转 0:反转
uint8_t motor_dir = 1;
// 电机编码器读取的电机速度,为正表示正转，为负表示反转
int16_t measure_motor_speed = 0;
// 电机ID
uint8_t motor_id = 0;
// 电机速度,正数表示正转，负数表示反转
int16_t set_motor_speed = 0;
// 上一次霍尔值
uint16_t last_hall_status = 0;
// 当前霍尔值
uint16_t current_hall_status = 0;
// 定时器溢出次数
uint32_t timer_overflow_count = 0;
// 定时器最终溢出次数
uint32_t timer_overflow_final_count = 0;
// 霍尔状态变化次数
uint8_t hall_status_change_count = 0;

/*
 * @brief 启动BLDC电机控制系统
 * 
 * 该函数初始化并启动BLDC电机的控制流程，包括驱动使能、定时器中断配置、
 * PWM输出启动以及PID控制器初始化。最后设置一个短暂延时以确保系统稳定。
 *
 * @param 无
 * @return 无
 */
void Int_BLDC_Start(void)
{
    // 启动驱动
    HAL_GPIO_WritePin(YC_SD_GPIO_Port, YC_SD_Pin, GPIO_PIN_SET);

    // 启动定时器中断，用于周期性获取霍尔传感器的值
    __HAL_TIM_ENABLE_IT(&htim8, TIM_IT_UPDATE);
    // 开启PWM输出
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);

    // HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
    // HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_2);
    // HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_3);

    // 初始化PID控制器
    Int_PID_Init();

    // 等待10ms以确保电机平稳启动
    HAL_Delay(10);
    motor_status = 1;
}

/**
 * @brief 停止BLDC电机运行，安全关闭电机驱动和相关外设
 * 
 * 该函数执行以下操作：
 * 1. 停止电机驱动使能信号
 * 2. 关闭PWM定时器通道1-3
 * 3. 将PWM比较寄存器设置为0
 * 4. 将PWM输出引脚恢复到低电平
 * 5. 清除霍尔传感器状态和定时器溢出计数
 * 6. 更新电机状态为停止
 */
void Int_BLDC_Stop(void)
{
    // 缓慢关闭电机,防止冲击
    // if (pid_struct.output > 100)
    // {
    //     pid_struct.output -= 50;
    // }
    // 停止驱动
    HAL_GPIO_WritePin(YC_SD_GPIO_Port, YC_SD_Pin, GPIO_PIN_RESET);
    // 关闭定时器
    HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_3);
    // 清零占空比
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);
    // 恢复引脚状态
    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET);

    // 清空上次计算速度相关状态，避免影响下一次计算
    last_hall_status           = 0;
    current_hall_status        = 0;
    timer_overflow_count       = 0;
    timer_overflow_final_count = 0;

    motor_status = 0;
}

/**
 * @brief 控制电机
 *
 * @param set_nums  绝对值是占空比  正负表示正反转
 */
void Int_BLDC_Update_Speed(int16_t set_motor_speed)
{
    if (set_motor_speed >= 0)
    {
        // 正转
        motor_dir = 1;
    }
    else if (set_motor_speed < 0)
    {
        // 反转
        motor_dir = 0;
    }
}


uint8_t Int_BLDC_Get_Hall_Status(void)
{
    // 获取霍尔传感器的值
    uint8_t hall_status = (HAL_GPIO_ReadPin(HALL_U_GPIO_Port, HALL_U_Pin) << 2) |
                          (HAL_GPIO_ReadPin(HALL_V_GPIO_Port, HALL_V_Pin) << 1) |
                          (HAL_GPIO_ReadPin(HALL_W_GPIO_Port, HALL_W_Pin));
    return hall_status;
}

void Int_BLDC_UH_VL(void)
{
    // UH_VL
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pid_struct.output); // U
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_SET);   // V
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
}
void Int_BLDC_UH_WL(void)
{
    // UH_WL
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pid_struct.output); // U
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_SET);   // W
}
void Int_BLDC_VH_WL(void)
{ // VH_WL

    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pid_struct.output); // V
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_SET);   // W
}
void Int_BLDC_VH_UL(void)
{ // VH_UL

    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pid_struct.output); // V
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_SET);   // U
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
}
void Int_BLDC_WH_UL(void)
{
    // WH_UL
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pid_struct.output); // W

    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_SET);   // U
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
}
void Int_BLDC_WH_VL(void)
{
    // WH_VL
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pid_struct.output); // W

    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_SET);   // V
    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
}

// 正转控制数组
BLDC_Control cw[6] = {
    Int_BLDC_WH_VL,
    Int_BLDC_VH_UL,
    Int_BLDC_WH_UL,
    Int_BLDC_UH_WL,
    Int_BLDC_UH_VL,
    Int_BLDC_VH_WL,
};
// &反转控制数组
BLDC_Control ccw[6] = {
    Int_BLDC_VH_WL,
    Int_BLDC_UH_VL,
    Int_BLDC_UH_WL,
    Int_BLDC_WH_UL,
    Int_BLDC_VH_UL,
    Int_BLDC_WH_VL,
};

void Int_BLDC_Get_Current_Motor_Speed(void)
{
    measure_motor_speed = (1000 * 60 / (timer_overflow_final_count * 1.0 / 9));
    if (motor_dir == 0)
    {
        measure_motor_speed = -measure_motor_speed;
    }
}

/**
 * @brief 定时器周期中断回调函数
 *
 * 这是一个定时器周期中断的回调函数，由HAL库在定时器周期结束（溢出）时自动调用。
 * 该函数主要用于处理与无刷直流电机（BLDC）相关的换向逻辑，包括：
 * - 检测电机是否处于运行状态；
 * - 记录定时器溢出次数；
 * - 获取并处理霍尔传感器的状态变化；
 * - 根据当前方向执行对应的换向操作。
 *
 * 此函数通常用于需要动态响应的场合（如BLDC换向），使用的是`HAL_TIM_PWM_Start_IT()`启动方式，
 * 并启用了定时器更新中断以支持此回调函数。
 *
 * @param htim 定时器句柄，包含定时器的配置信息和状态
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    Int_BLDC_Get_Current_Motor_Speed();
    // Debug_printf("%d\n", measure_motor_speed);
    //  判断是否是TIM8定时器触发，并且电机处于运行状态（motor_status == 1）
    if (htim->Instance == TIM8 && motor_status == 1)
    {
        // 定时器溢出计数加1
        timer_overflow_count++;
        // 获取当前霍尔传感器状态
        current_hall_status = Int_BLDC_Get_Hall_Status();
        // 如果当前霍尔状态与上一次不同，则说明霍尔信号发生变化
        if (current_hall_status != last_hall_status)
        {
            // 霍尔状态变化次数加1
            hall_status_change_count++;
            // 当霍尔状态变化达到一定次数（12次）时，进行相关参数重置
            if (hall_status_change_count == 12)
            {
                // 保存当前的定时器溢出次数到最终计数变量中
                timer_overflow_final_count = timer_overflow_count;
                // 重置定时器溢出次数计数器，以便下一轮计算
                timer_overflow_count = 0;
                // 重置霍尔状态变化次数计数器
                hall_status_change_count = 0;
            }

            // 更新last_hall_status为当前霍尔状态，供下次比较使用
            last_hall_status = current_hall_status;

            // 确保当前霍尔状态值有效（范围：1~6）
            if (current_hall_status > 0 && current_hall_status < 7)
            {
                // 根据电机的方向（motor_dir）选择正转或反转的换向表
                if (motor_dir == 1) // 正转方向
                {
                    // 执行正转方向对应的换向操作，通过函数指针数组cw调用对应位置的函数
                    cw[current_hall_status - 1]();
                }
                else if (motor_dir == 0) // 反转方向
                {
                    // 执行反转方向对应的换向操作，通过函数指针数组ccw调用对应位置的函数
                    ccw[current_hall_status - 1]();
                }
            }
        }
    }
}

// 复杂写法，加了后缀_Complex，若要启用，需要删掉后缀
void HAL_TIM_PeriodElapsedCallback_Complex(TIM_HandleTypeDef *htim)
{

    // 判断电机是否在运行
    if (htim->Instance == TIM8 && motor_status == 1)
    {
        // 获取霍尔状态
        uint8_t hall_status = Int_BLDC_Get_Hall_Status();
        if (hall_status > 0 && hall_status < 7)
        {
            // printf("hall_status:%d\r\n", hall_status);
            switch (hall_status)
            {
                // 101  UH_VL
                case 0x05:
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pid_struct.output); // U
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

                    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
                    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_SET);   // V
                    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
                    break;
                // 100 UH_WL
                case 0x04:
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pid_struct.output); // U
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

                    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
                    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
                    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_SET);   // W
                    break;
                // 110 VH_WL
                case 0x06:
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pid_struct.output); // V
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

                    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
                    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
                    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_SET);   // W
                    break;
                // 010 VH_UL
                case 0x02:
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pid_struct.output); // V
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);                 // W

                    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_SET);   // U
                    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
                    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
                    break;
                // 011 WH_UL
                case 0x03:
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pid_struct.output); // W

                    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_SET);   // U
                    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_RESET); // V
                    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
                    break;
                // 001 WH_VL
                case 0x01:
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);                 // U
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);                 // V
                    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pid_struct.output); // W

                    HAL_GPIO_WritePin(PWM_U_L_GPIO_Port, PWM_U_L_Pin, GPIO_PIN_RESET); // U
                    HAL_GPIO_WritePin(PWM_V_L_GPIO_Port, PWM_V_L_Pin, GPIO_PIN_SET);   // V
                    HAL_GPIO_WritePin(PWM_W_L_GPIO_Port, PWM_W_L_Pin, GPIO_PIN_RESET); // W
                    break;
            }
        }
    }
}

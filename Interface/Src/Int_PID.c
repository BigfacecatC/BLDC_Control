#include "Int_PID.h"

// PID控制器结构体实例
PID_t pid_struct = {
    .kp = 0.001,
    .ki = 0.0025,
    .kd = 0.00,
};

/**
 * @brief PID初始化函数
 *
 */
void Int_PID_Init(void)
{
    // 初始化当前误差为0
    pid_struct.error = 0;
    // 初始化上一次误差为0
    pid_struct.error_last = 0;
    // 初始化输出值为50，让电机在启动时有转动
    pid_struct.output = 50;
}

/**
 * @brief 计算PID控制输出
 */
void Int_PID_Calculate(void)
{
    // 取绝对值做误差计算，即实际速度和目标速度的差值
    pid_struct.error = abs(set_motor_speed) - abs(measure_motor_speed);
    // 误差累积
    pid_struct.error_sum += pid_struct.error;
    // 给定初始error_last值，防止开始时error_last为0，造成开始误差太大
    if (pid_struct.error_last < 0)
    {
        pid_struct.error_last = pid_struct.error;
    }
    // 计算PID输出
    pid_struct.output = pid_struct.kp * pid_struct.error + pid_struct.ki * pid_struct.error_sum + pid_struct.kd * (pid_struct.error - pid_struct.error_last);
    // 给定error_last赋值，为下次计算做准备
    pid_struct.error_last = pid_struct.error;
}

/**
 * @brief  定时执行PID控制计算函数
 */
void HAL_IncTick(void)
{
    uwTick += uwTickFreq;
    // 间隔50ms并且电机正在转动才执行PID计算,
    if (uwTick % 10 == 0 && motor_status != 0)
    {
        // PID计算
        Int_PID_Calculate();
        // 限制输出范围
        pid_struct.output = LIMIT(pid_struct.output, 50, 1000);
        printf("%d,%d,%d\n", (uint16_t)pid_struct.output, set_motor_speed, measure_motor_speed);
    }
}

#include "Com_Debug.h"
void Com_Debug_Init(void)
{
    // 主函数已经初始化uart1，所以这里不需要再初始化uart1
    // MX_USART1_UART_Init();
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
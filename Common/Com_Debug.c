#include "Com_Debug.h"
void Com_Debug_Init(void)
{
    // �������Ѿ���ʼ��uart1���������ﲻ��Ҫ�ٳ�ʼ��uart1
    // MX_USART1_UART_Init();
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
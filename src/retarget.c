#include "stm32f1xx_hal.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;  // استبدل بـ huart2 لو شغال على USART2

int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
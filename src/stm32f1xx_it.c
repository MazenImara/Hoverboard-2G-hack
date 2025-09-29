#include "stm32f1xx_hal.h"


extern DMA_HandleTypeDef hdma_adc1;

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc1);
}
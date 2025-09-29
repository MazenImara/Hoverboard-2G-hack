#include "melody.h"

extern TIM_HandleTypeDef htim3;

void power_on_melody(void)
{
    int notes[] = {2000, 1500, 1800};
    int durations[] = {200, 150, 300};
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    for (int i = 0; i < 3; i++)
    {
        int period = 72000000 / (htim3.Init.Prescaler + 1) / notes[i];
        __HAL_TIM_SET_AUTORELOAD(&htim3, period);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, period / 2);
        HAL_Delay(durations[i]);
    }

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
}

void power_off_melody(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_Delay(150);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
    HAL_Delay(100);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_Delay(150);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
}


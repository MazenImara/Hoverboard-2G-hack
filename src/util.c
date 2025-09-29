#include "util.h"

extern ADC_HandleTypeDef hadc1;

uint32_t throttleValue;

void readThrottle(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    sConfig.Channel = ADC_CHANNEL_6;   // PA6 = ADC_IN6
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    throttleValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
}
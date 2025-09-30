#include "util.h"


extern uint16_t adcValues[ADC_CHANNEL_COUNT];

float readBatteryVoltage(void)
{
    const float Vmin = 30.0f;
    const float Vmax = 42.0f;
    const int ADCmin = 2308;
    const int ADCmax = 3335;

    int adcValue = adcValues[2]; // قراءة من القناة المطلوبة

    // حماية ضد القيم الخارجة عن النطاق
    if (adcValue < ADCmin) adcValue = ADCmin;
    if (adcValue > ADCmax) adcValue = ADCmax;

    // معادلة التحويل
    float voltage = Vmin + ((adcValue - ADCmin) * (Vmax - Vmin)) / (float)(ADCmax - ADCmin);

    return voltage;
}

/*
l = 2308
h = 3335

*/
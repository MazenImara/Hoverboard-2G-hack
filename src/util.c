#include "util.h"

// التعريفات
#define PI 3.14159265f
#define VBUS 12.0f // جهد البطارية
#define PWM_PERIOD (64000000 / 2 / 16000)  // نفس اللي حددته في TIM1
#define PWM_AMPLITUDE (PWM_PERIOD / 2.0f)  // نصف الموجة تقريبًا


extern uint16_t adcValues[ADC_CHANNEL_COUNT];
// PWM on Low-side (N outputs are TIM1_CH1N, CH2N, CH3N)
extern TIM_HandleTypeDef htim1;

uint16_t pwm_duty = 0;

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

float readInternalTemperature(void)
{
    float Vsense = (adcValues[3] / 4095.0f) * 3.3f;
    float temperature = ((1.43f - Vsense) / 0.0043f) + 25.0f;
    return temperature;
}

uint8_t readHallState(void)
{
    uint8_t hallA = HAL_GPIO_ReadPin(HALL_PORT, HALL_A_PIN);
    uint8_t hallB = HAL_GPIO_ReadPin(HALL_PORT, HALL_B_PIN);
    uint8_t hallC = HAL_GPIO_ReadPin(HALL_PORT, HALL_C_PIN);

    uint8_t state = (hallC << 2) | (hallB << 1) | hallA;
    return state;
}

void testHallSensors(void)
{
    uint8_t lastState = 0xFF;  // قيمة غير صالحة بالبداية

    while (1)
    {
        uint8_t state = readHallState();
        if (state != lastState)
        {
            printf("Hall state: %d\r\n", state);
            lastState = state;
        }

        HAL_Delay(50); // تأخير صغير لتسهيل القراءة
    }
}

uint16_t getThrottlePercent(void)
{
    uint16_t raw = adcValues[0]; // قيمة ADC
    // نفترض أن ADC من 0 إلى 4095

    // تعيين حدود الدواسة (تعدل حسب جهازك)
    uint16_t min_adc = 870;  // أقل قيمة تعتبر بداية
    uint16_t max_adc = 2818; // أعلى قيمة للدواسة

    if (raw < min_adc)
        return 0;  // أقل من الحد الأدنى → 0%

    if (raw > max_adc)
        return 100; // أعلى من الحد الأعلى → 100%

    // حساب النسبة مابين 0 و 100
    uint32_t percent = (uint32_t)(raw - min_adc) * 100 / (max_adc - min_adc);

    return (uint8_t)percent;
}



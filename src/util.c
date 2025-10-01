#include "util.h"




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

float getCurrentAmps(void)
{
    uint16_t raw = adcValues[1];
    float voltage = (3.3f * (float)raw) / 4095.0f;
    float voltage_shunt = voltage / 10.0f; // إذا كان هناك مكبر gain=10
    float current = voltage_shunt / 0.005f;

    // تعويض الانحراف
    current -= CURRENT_OFFSET;

    // حماية من القراءة السالبة
    if (current < 0.0f) current = 0.0f;

    return current;
}

void currentProtect(void)
{
    float currentA = getCurrentAmps();
    if (currentA > OVERCURRENT_LIMIT)
    {
        stopAllMotorOutputs();  // ← تطفئ المحرك تمامًا
        printf("⚠️  Overcurrent detected! Current = %.2f A -> Stopping motor!\r\n", currentA);
        
        powerOff();
    }    
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

void stopAllMotorOutputs(void)
{
    // وقف القنوات الرئيسية (High side)
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

    // وقف القنوات المكملة (Low side)
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

    // إعادة المقارنات إلى الصفر (PWM = 0%)
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
}

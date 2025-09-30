#include "util.h"


extern uint16_t adcValues[ADC_CHANNEL_COUNT];
// PWM on Low-side (N outputs are TIM1_CH1N, CH2N, CH3N)
extern TIM_HandleTypeDef htim1;

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

void readHallSensors(void)
{
    uint8_t hallGA = HAL_GPIO_ReadPin(HALL_PORT, HALL_A_PIN);  // Green
    uint8_t hallYB = HAL_GPIO_ReadPin(HALL_PORT, HALL_B_PIN);  // Yellow
    uint8_t hallBC = HAL_GPIO_ReadPin(HALL_PORT, HALL_C_PIN);  // Blue

    uint8_t hallState = (hallGA << 2) | (hallYB << 1) | hallBC;

    printf("Hall A: %d, Hall B: %d, Hall C: %d => State: %d\r\n", hallGA, hallYB, hallBC, hallState);

    HAL_Delay(200);  // للتقليل من سرعة الطباعة
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
void set_commutation(uint8_t hall_state)
{
    // أولًا: أوقف كل high-sides
    HAL_GPIO_WritePin(PHASE_U_HIGH_PORT, PHASE_U_HIGH_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PHASE_V_HIGH_PORT, PHASE_V_HIGH_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PHASE_W_HIGH_PORT, PHASE_W_HIGH_PIN, GPIO_PIN_RESET);

    // ثم: أوقف كل low-sides PWM
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

    // بناءً على الحالة، فعّل المطلوب
    switch(hall_state)
    {
        case 1: // U+ V-
            HAL_GPIO_WritePin(PHASE_U_HIGH_PORT, PHASE_U_HIGH_PIN, GPIO_PIN_SET);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);  // V-
            break;

        case 2: // U+ W-
            HAL_GPIO_WritePin(PHASE_U_HIGH_PORT, PHASE_U_HIGH_PIN, GPIO_PIN_SET);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);  // W-
            break;

        case 3: // V+ W-
            HAL_GPIO_WritePin(PHASE_V_HIGH_PORT, PHASE_V_HIGH_PIN, GPIO_PIN_SET);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);  // W-
            break;

        case 4: // V+ U-
            HAL_GPIO_WritePin(PHASE_V_HIGH_PORT, PHASE_V_HIGH_PIN, GPIO_PIN_SET);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);  // U-
            break;

        case 5: // W+ U-
            HAL_GPIO_WritePin(PHASE_W_HIGH_PORT, PHASE_W_HIGH_PIN, GPIO_PIN_SET);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);  // U-
            break;

        case 6: // W+ V-
            HAL_GPIO_WritePin(PHASE_W_HIGH_PORT, PHASE_W_HIGH_PIN, GPIO_PIN_SET);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);  // V-
            break;

        default: // Invalid state
            break;
    }
}


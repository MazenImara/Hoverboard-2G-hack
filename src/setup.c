
#include "setup.h"
#include "defines.h"


UART_HandleTypeDef huart1;
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim1;
uint16_t adcValues[ADC_CHANNEL_COUNT];  // مصفوفة تستقبل القيم من DMA


void MX_GPIO_Init(void) {

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /** ========== output mode ========= **/
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = FRONT_LED_PIN;
  HAL_GPIO_Init(FRONT_LED_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = SELF_HOLD_PIN;
  HAL_GPIO_Init(SELF_HOLD_PORT, &GPIO_InitStruct);

  /** ========== input mode ========= **/
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = POWER_BTN_PIN;
  HAL_GPIO_Init(POWER_BTN_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = BREAK_PIN;
  HAL_GPIO_Init(BREAK_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : for hall sensor */
  GPIO_InitStruct.Pin = HALL_A_PIN | HALL_B_PIN | HALL_C_PIN;
  HAL_GPIO_Init(HALL_PORT, &GPIO_InitStruct);

  /** ========== Alternate Function mode (PWM) ========= **/
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // Alternate Function Push-Pull
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = BUZZER_PIN;
  HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);

  /*Configure GPIO pins : for motor */
  // إعداد Pins القنوات العادية (High-side): PA8, PA9, PA10
  GPIO_InitStruct.Pin = PHASE_U_HIGH_PIN | PHASE_V_HIGH_PIN | PHASE_W_HIGH_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // إعداد Pins القنوات التكميلية (Low-side): PB13, PB14, PB15
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);   

  /** ========== Analog mode ========= **/
  __HAL_RCC_ADC1_CLK_ENABLE();
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = THROTTLE_PIN;
  HAL_GPIO_Init(THROTTLE_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = BATTERY_V_PIN;
  HAL_GPIO_Init(BATTERY_V_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = CURRENT_PIN;
  HAL_GPIO_Init(CURRENT_PORT, &GPIO_InitStruct);

}

void MX_UART1_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

void MX_ADC1_Init(void)
{
  __HAL_RCC_ADC1_CLK_ENABLE();

  ADC_ChannelConfTypeDef sConfig = {0};
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;  // مهم: لتفعيل المسح المتعدد
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = ADC_CHANNEL_COUNT;
  HAL_ADC_Init(&hadc1);
  
  // تفعيل مستشعر الحرارة الداخلي
  ADC1->CR2 |= ADC_CR2_TSVREFE;

  // قناة 1: Throttle (PA6 = ADC_CHANNEL_6)
  sConfig.Channel = THROTTLE_CH;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // قناة 2: Current (مثلاً PA2 = ADC_CHANNEL_2)
  sConfig.Channel = CURRENT_CH;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // قناة 3: Battery Voltage (مثلاً PA5 = ADC_CHANNEL_5)
  sConfig.Channel = BATTERY_V_CH;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // قناة 4: Temperature (مثلاً PA3 = ADC_CHANNEL_3)
  sConfig.Channel = ADC_CHANNEL_16;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    // 7. تشغيل ADC باستخدام DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValues, ADC_CHANNEL_COUNT);
}

void MX_TIM3_Init(void)
{
  __HAL_RCC_TIM3_CLK_ENABLE();  // تفعيل ساعة المؤقت
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72 - 1;           // (72MHz / 72) = 1MHz
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 500 - 1;             // 1MHz / 500 = 2kHz → صوت متوسط
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim3);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 250;                   // 50% duty cycle
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
}

void MX_TIM1_Init(void)
{
  __HAL_RCC_TIM1_CLK_ENABLE();

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler         = 0;
  htim1.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period            = 64000000 / 2 / 16000;
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
  HAL_TIM_PWM_Init(&htim1);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

    // إعداد قناة PWM
  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.Pulse        = 0;
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_LOW;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);

  // إعداد الـ Dead Time و Break (لحماية MOSFETs)
  sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime         = 48;
  sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_LOW;
  sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;

  HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);
}

void Start_PWM_TIM1(void)
{  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  printf("after HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1)\r\n");

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

  printf("after HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1)\r\n");


  __HAL_TIM_ENABLE(&htim1);
}
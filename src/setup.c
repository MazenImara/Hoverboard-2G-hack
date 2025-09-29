
#include "setup.h"
#include "defines.h"


UART_HandleTypeDef huart1;
ADC_HandleTypeDef hadc1;


void MX_GPIO_Init(void) {

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // Output mode
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = FRONT_LED_PIN;
  HAL_GPIO_Init(FRONT_LED_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = BUZZER_PIN;
  HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = SELF_HOLD_PIN;
  HAL_GPIO_Init(SELF_HOLD_PORT, &GPIO_InitStruct);



  // Input mode
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = POWER_BTN_PIN;
  HAL_GPIO_Init(POWER_BTN_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = BREAK_PIN;
  HAL_GPIO_Init(BREAK_PORT, &GPIO_InitStruct);
  

  // Analog
  __HAL_RCC_ADC1_CLK_ENABLE();
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;

    /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = THROTTLE_PIN;
  HAL_GPIO_Init(THROTTLE_PORT, &GPIO_InitStruct);

}

void MX_UART1_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);
}
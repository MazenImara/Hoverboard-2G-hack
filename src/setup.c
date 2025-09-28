
#include "setup.h"
#include "defines.h"


UART_HandleTypeDef huart1;


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



  // Input mode
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;

  /*Configure GPIO pin : P */
  GPIO_InitStruct.Pin = POWER_BTN_PIN;
  HAL_GPIO_Init(POWER_BTN_PORT, &GPIO_InitStruct);


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
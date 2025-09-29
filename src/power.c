#include "power.h"

void power_on(void)
{
  HAL_GPIO_WritePin(SELF_HOLD_PORT, SELF_HOLD_PIN, GPIO_PIN_SET);
}

void power_off(void)
{
  while (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN)){}
  HAL_GPIO_WritePin(SELF_HOLD_PORT, SELF_HOLD_PIN, GPIO_PIN_RESET);
}
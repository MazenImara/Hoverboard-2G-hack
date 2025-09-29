#include "power.h"

void power_on(void)
{
  HAL_GPIO_WritePin(SELF_HOLD_PORT, SELF_HOLD_PIN, GPIO_PIN_SET);
  power_on_melody();
}

void power_off(void)
{
  while (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN)){}
  power_off_melody();
  HAL_GPIO_WritePin(SELF_HOLD_PORT, SELF_HOLD_PIN, GPIO_PIN_RESET);
}
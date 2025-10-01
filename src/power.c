#include "power.h"

void powerOn(void)
{
  HAL_GPIO_WritePin(SELF_HOLD_PORT, SELF_HOLD_PIN, GPIO_PIN_SET);
  power_on_melody();
}

void powerOff(void)
{  
  power_off_melody();
  HAL_GPIO_WritePin(SELF_HOLD_PORT, SELF_HOLD_PIN, GPIO_PIN_RESET);
}
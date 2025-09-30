#define SETUP_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "defines.h"


void MX_GPIO_Init(void);
void MX_UART1_Init(void);
void MX_ADC1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM1_Init(void);
void Start_PWM_TIM1(void);

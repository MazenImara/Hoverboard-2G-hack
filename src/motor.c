#include "motor.h"

extern TIM_HandleTypeDef htim1;            // باراميترات الكنترولر

int dutyA, dutyB, dutyC;
float current, batteryVolt;

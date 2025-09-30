#define UTIL_H

#include "setup.h"

float readBatteryVoltage(void);
float readInternalTemperature(void);
uint16_t getThrottlePercent(void);
void readHallSensors(void);
void set_commutation(uint8_t hall_state);
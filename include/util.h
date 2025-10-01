#define UTIL_H

#include "setup.h"
#include <math.h>

float readBatteryVoltage(void);
float readInternalTemperature(void);
uint16_t getThrottlePercent(void);
uint8_t readHallState(void);
void loopFOC_HallBased(void);
void commutate(uint8_t hallState, uint16_t duty);
void testHallSensors(void);
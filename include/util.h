#define UTIL_H

#include "setup.h"
#include <math.h>
#include "power.h"
#include <stdio.h>
#include <string.h>

float readBatteryVoltage(void);
float readInternalTemperature(void);
uint16_t getThrottlePercent(void);
uint8_t readHallState(void);
void setPhaseVoltage(float angle_el, float voltage);
void testHallSensors(void);

float getCurrentAmps(void);

void stopAllMotorOutputs(void);
void currentProtect(void);
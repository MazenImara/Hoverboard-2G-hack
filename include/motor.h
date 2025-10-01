#define MOTOR_H


#include "BLDC_controller.h"
#include "util.h"
#include "setup.h"
#include "defines.h"


void setupBLDC(void);
void doMotorControl(void);
uint16_t mapDuty(int16_t duty);
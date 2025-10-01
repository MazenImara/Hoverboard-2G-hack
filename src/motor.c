#include "motor.h"

extern TIM_HandleTypeDef htim1;
extern P rtP_Left;

static RT_MODEL rtM_;
static DW rtDW;
static ExtU rtU;
static ExtY rtY;
static P rtP;                 // باراميترات الكنترولر

int dutyA, dutyB, dutyC;
float current, batteryVolt;

void setupBLDC(void)
{
  rtM_.defaultParam = &rtP;
  rtM_.inputs = &rtU;
  rtM_.outputs = &rtY;
  rtM_.dwork = &rtDW;
  rtM_.defaultParam = &rtP_Left;

  BLDC_controller_initialize(&rtM_);
}

void doMotorControl(void)
{
    // Get hall sensors values
  uint8_t hall_ul = HAL_GPIO_ReadPin(HALL_PORT, HALL_A_PIN);
  uint8_t hall_vl = HAL_GPIO_ReadPin(HALL_PORT, HALL_B_PIN);
  uint8_t hall_wl = HAL_GPIO_ReadPin(HALL_PORT, HALL_C_PIN);

  // ==== تحديث المدخلات للكنترولر ====
  rtU.b_motEna = 1;     // دايمًا شغّال (ممكن تربطه بزرّ)
  rtU.z_ctrlModReq = 2; // اختار المود (مثلاً 2 = Torque Mode)
  rtU.r_inpTgt = (int16_t)getThrottlePercent();
/* 
  uint8_t hallState = readHallState();
  rtU.b_hallA = (hallState & 0x01) ? 1 : 0;
  rtU.b_hallB = (hallState & 0x02) ? 1 : 0;
  rtU.b_hallC = (hallState & 0x04) ? 1 : 0;
 */
  rtU.b_hallA = hall_ul;
  rtU.b_hallB = hall_vl;
  rtU.b_hallC = hall_wl;
  rtU.i_DCLink = (int16_t)(getCurrentAmps() * 100.0f); // تحجيم للـint16
  rtU.a_mechAngle = 0;                                 // لو ما عندك مستشعر زاوية

  // ==== نداء الكنترولر ====
  BLDC_controller_step(&rtM_);

  dutyA = rtY.DC_phaA;
  dutyB = rtY.DC_phaB;
  dutyC = rtY.DC_phaC;

  // ==== استخدام المخرجات ====
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, mapDuty(dutyA));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mapDuty(dutyB));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mapDuty(dutyC));

  // Debug كل 100ms
  static uint32_t lastTick = 0;
  if (HAL_GetTick() - lastTick > 1000)
  {
    lastTick = HAL_GetTick();
    current = getCurrentAmps();
    batteryVolt = readBatteryVoltage();
    printf("Vbat=%d.%02dV, I=%d.%02dA, Thr=%d%%, n_mot=%d\r\n",
           (int)batteryVolt, (int)((batteryVolt - (int)batteryVolt) * 100),
           (int)current, (int)((current - (int)current) * 100),
           getThrottlePercent(),
           rtY.n_mot);
    printf("duty A: %d, B: %d, C: %d \r\n", dutyA, dutyB, dutyC);
    printf("mapDuty A: %d, B: %d, C: %d \r\n", mapDuty(dutyA), mapDuty(dutyB), mapDuty(dutyC));           

  }

  HAL_Delay(1);
}

// تحويل قيمة Duty من خرج الكنترولر إلى CCR (PWM register)
uint16_t mapDuty(int16_t duty)
{
  // فرضاً الخرج من Simulink هو -32768..32767
  // نطبّعه إلى 0..PWM_PERIOD
  int32_t pwmPeriod = __HAL_TIM_GET_AUTORELOAD(&htim1);
  int32_t scaled = ((int32_t)duty + 32768) * pwmPeriod / 65535;
  if (scaled < 0)
  {
    scaled = 0;
  }
  if (scaled > pwmPeriod)
  {
    scaled = pwmPeriod;
  }
  return (uint16_t)scaled;
}
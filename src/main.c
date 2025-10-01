#include "main.h"

void SystemClock_Config(void);

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern uint16_t adcValues[ADC_CHANNEL_COUNT];
extern TIM_HandleTypeDef htim1;


float batteryVolt = 0;
float temperature = 0;

#define PI 3.14159265f
float angle = 0.0f;
void loopFOC_OpenTest()
{
    angle += 2.0f * PI * 0.005f; // السرعة الزاوية
    if (angle >= 2.0f * PI) angle -= 2.0f * PI;
    setPhaseVoltage(angle, 6.0f); // جهد نصف VBUS مثلاً
    HAL_Delay(1);
}


void startupSequence(float duty, uint16_t steps, uint16_t delayMs)
{
    // تسلسل الحالات حسب الاتجاه الصحيح الذي رأيناه
    uint8_t hallSequence[6] = {6, 4, 5, 1, 3, 2};

    for (int i = 0; i < steps; i++)
    {
        uint8_t hall = hallSequence[i % 6];
        commutate_safe(hall, duty);
        HAL_Delay(delayMs);
    }
}


int main(void)
{
  HAL_Init();               
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_UART1_Init();

  printf("\r\n");

  printf("\r\n");

  printf("\r\n");

  printf("before MX_TIM1_Init\r\n");
  MX_TIM1_Init();

  printf("before MX_TIM3_Init\r\n");
  MX_TIM3_Init();


  power_on();

  HAL_Delay(3000);

  printf("before Start_PWM_TIM1\r\n");
  Start_PWM_TIM1();





  int printTimer = 0;
  printf("start Loop\r\n");
  while(1)
  {


    printTimer++;
    if (printTimer > 100)
    {
      printTimer = 0;

      batteryVolt = readBatteryVoltage();
      temperature = readInternalTemperature();
/* 
      printf("temperture.    : %d.%02d C\r\n", (int)temperature, (int)((temperature - (int)temperature) * 100));
      printf("Battery Voltage: %d.%02d V\r\n", (int)batteryVolt, (int)((batteryVolt - (int)batteryVolt) * 100));
      printf("Current        : %lu A\r\n", adcValues[1]); 
      printf("Throttle percent: %i \r\n", getThrottlePercent());
       */
      //printf("hall state: %i \r\n", readHallState());

    }

    
        

    testHallSensors();






    if (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN) 
/* 
      || batteryVolt < MIN_BATTERY_VOL 
      || batteryVolt > MAX_BATTERY_VOL
      || temperature > MAX_TEMPERATURE
       */
    )
    {
      power_off();
    }
        
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}



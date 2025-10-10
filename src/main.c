#include "main.h"

void SystemClock_Config(void);

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;


float batteryVolt = 0;
float temperature = 0;
float current = 0;

int printTimer = 0;
int printDelay = 5000;

int main(void)
{
  HAL_Init();               
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_UART1_Init();
  
  

  for (size_t i = 0; i < 5; i++)
  {
    printf("\r\n");
  }
  

  printf("before MX_TIM1_Init\r\n");
  MX_TIM1_Init();

  printf("before MX_TIM3_Init\r\n");
  MX_TIM3_Init();  
  powerOn();

  HAL_Delay(3000);

  printf("before Start_PWM_TIM1\r\n");
  Start_PWM_TIM1();

  printf("before setupBLDC\r\n");
  setupBLDC();


  printf("start Loop\r\n");
  while(1)
  {
    printTimer++;
    if (printTimer > printDelay)
    {
      batteryVolt = readBatteryVoltage();
      temperature = readInternalTemperature();
      current = getCurrentAmps();

      printf("temperture.    : %d.%02d C\r\n", (int)temperature, (int)((temperature - (int)temperature) * 100));
      printf("Battery Voltage: %d.%02d V\r\n", (int)batteryVolt, (int)((batteryVolt - (int)batteryVolt) * 100));
      printf("Current        : %d.%02d \r\n", (int)current, (int)((current - (int)current) * 100));
      
      printf("Throttle percent: %i \r\n", getThrottlePercent());
       
      //printf("hall state: %i \r\n", readHallState());

    }



    currentProtect();
    if (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN) 
/* 
      || batteryVolt < MIN_BATTERY_VOL 
      || batteryVolt > MAX_BATTERY_VOL
      || temperature > MAX_TEMPERATURE
       */
    )
    {
      while (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN)){}
      powerOff();
    }
    if (printTimer > printDelay) { printTimer = 0; }     
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



#include "main.h"

void SystemClock_Config(void);

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern uint16_t adcValues[ADC_CHANNEL_COUNT];

float batteryVolt = 0;
float temperature = 0;

int main(void)
{
  HAL_Init();               
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_UART1_Init();
  MX_TIM3_Init();


  power_on();




  printf("start Loop\r\n");
  while(1)
  {
       batteryVolt = readBatteryVoltage();
       temperature = readInternalTemperature();
       //printf("Battery Voltage: %d.%02d V\r\n", (int)batteryVolt, (int)((batteryVolt - (int)batteryVolt) * 100));
       //printf("temperture: %d.%02d C\r\n", (int)temperature, (int)((temperature - (int)temperature) * 100));
       printf("Current Sensor ADC Value: %lu\n", adcValues[1]);
       //printf("tmp: %lu \r\n", temperature);
    HAL_Delay(500);
    //printf("adcValues[2] raw: %i \r\n", adcValues[2]);
    //printf("power btn: %.2f \r\n", HAL_GPIO_ReadPin(BREAK_PORT, BREAK_PIN));




    if (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN) 
      || batteryVolt < MIN_BATTERY_VOL 
      || batteryVolt > MAX_BATTERY_VOL
      || temperature > MAX_TEMPERATURE
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



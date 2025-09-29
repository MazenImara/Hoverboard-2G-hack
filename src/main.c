#include "main.h"

void SystemClock_Config(void);

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

uint32_t throttleValue = 0;
uint32_t brakeValue = 0;

int main(void)
{
  HAL_Init();               
  SystemClock_Config();
  MX_GPIO_Init();
  MX_UART1_Init();
  MX_ADC1_Init();
  printf("start Loop\r\n");
  while(1)
  {
        // قراءة دواسة التيرتل (PC15)
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;

    // قراءة دواسة البريك (PA6)
    sConfig.Channel = ADC_CHANNEL_6;   // PA6 = ADC_IN6
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    throttleValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    // الآن يمكنك استخدام throttleValue و brakeValue حسب التطبيق
    printf("Throttle: %lu, break: %lu \r\n", throttleValue, brakeValue);
    HAL_Delay(1000);  // تأخير بسيط
    printf("power btn: %lu \r\n", HAL_GPIO_ReadPin(BREAK_PORT, BREAK_PIN));
    if (HAL_GPIO_ReadPin(POWER_BTN_PORT, POWER_BTN_PIN))
    {
      HAL_GPIO_WritePin(FRONT_LED_PORT, FRONT_LED_PIN, GPIO_PIN_SET);
      //printf("Hello from STM32!\r\n");
      HAL_Delay(1000);
    }
    else
    {
      HAL_GPIO_WritePin(FRONT_LED_PORT, FRONT_LED_PIN, GPIO_PIN_RESET);
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


int _write(int file, char *data, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t*)data, len, HAL_MAX_DELAY);
    return len;
}
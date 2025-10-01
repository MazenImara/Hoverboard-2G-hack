#define DEFINES_H

#define ADC_CHANNEL_COUNT 4  // عدد القنوات

#define MIN_BATTERY_VOL 30.1
#define MAX_BATTERY_VOL 41.9

#define MAX_TEMPERATURE 64.9

#define PI    3.1415926f
#define VBUS  36.0f  // ✅ مهم: فولت البطارية
#define CURRENT_OFFSET 0.95f  // حسب ملاحظتك في وضع السكون
#define OVERCURRENT_LIMIT  15.0f


#define FRONT_LED_PIN GPIO_PIN_13
#define FRONT_LED_PORT GPIOC

#define BUZZER_PIN GPIO_PIN_1
#define BUZZER_PORT GPIOB

#define POWER_BTN_PIN GPIO_PIN_3
#define POWER_BTN_PORT GPIOA

#define SELF_HOLD_PIN GPIO_PIN_4
#define SELF_HOLD_PORT GPIOA

#define THROTTLE_PIN GPIO_PIN_6
#define THROTTLE_PORT GPIOA
#define THROTTLE_CH ADC_CHANNEL_6

#define BREAK_PIN GPIO_PIN_15
#define BREAK_PORT GPIOC

#define BATTERY_V_PIN GPIO_PIN_5
#define BATTERY_V_PORT GPIOA
#define BATTERY_V_CH ADC_CHANNEL_5

#define CURRENT_PIN GPIO_PIN_7
#define CURRENT_PORT GPIOA
#define CURRENT_CH ADC_CHANNEL_7

// Hall pins
#define HALL_A_PIN    GPIO_PIN_0
#define HALL_B_PIN    GPIO_PIN_1
#define HALL_C_PIN    GPIO_PIN_2
#define HALL_PORT     GPIOA


// High-side phases
#define PHASE_U_HIGH_PIN  GPIO_PIN_10
#define PHASE_V_HIGH_PIN  GPIO_PIN_9
#define PHASE_W_HIGH_PIN  GPIO_PIN_8
#define PHASE_HIGH_PORT GPIOA


/* 
green high -> PA10
green low -> PB15

blue high -> PA9
blue low -> PB14

yallow high -> PA8
yallo low -> PB13

 */
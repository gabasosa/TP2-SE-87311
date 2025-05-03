#ifndef _PIN_MAP_H_
#define _PIN_MAP_H_

// === Sensors ===
#define TEMP_HUM_SENSOR_PIN             D4

// === Actuators ===
#define COOLER_PWM_PIN                  D6
#define COOLER_PWM_LED_INDICATOR        LED1
#define HUMIDIFIER_RELAY_PIN            D5
#define HUMIDIFIER_LED_INDICATOR        LED2
#define FAN_CONTROL_PIN                 D7
#define FAN_LED_INDICATOR               LED3
#define SERVO_FLAP_PIN                  D9

// === LEDs ===
#define HEATER_LED_PIN                  LED1
#define LIGHT_LED_PIN                   LED2

// === Matrix keypad ===
// Keypad row outputs
#define KEYPAD_ROW_0_PIN  PG_1
#define KEYPAD_ROW_1_PIN  PF_9
#define KEYPAD_ROW_2_PIN  PF_7
#define KEYPAD_ROW_3_PIN  PF_8
// Keypad column inputs
#define KEYPAD_COL_0_PIN  PE_3
#define KEYPAD_COL_1_PIN  PE_6
#define KEYPAD_COL_2_PIN  PE_5
#define KEYPAD_COL_3_PIN  PE_4

// === Display (SPI) ===
#define DISPLAY_CS_PIN    PA_15
#define DISPLAY_DC_PIN    PB_6
#define DISPLAY_RST_PIN   PB_8

#endif
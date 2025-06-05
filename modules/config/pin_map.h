#ifndef _PIN_MAP_H_
#define _PIN_MAP_H_

// === Sensors ===
#define TEMP_HUM_SENSOR_PIN             D4

// === Actuators ===
#define COOLER_PWM_PIN                  D6
#define COOLER_PWM_LED_INDICATOR        LED1
#define HUMIDIFIER_RELAY_PIN            D7
#define HUMIDIFIER_LED_INDICATOR        LED2
#define FAN_RELAY_PIN                   D8
#define FAN_LED_INDICATOR               LED3
#define SERVO_FLAP_PIN                  D9

// === LEDs ===
#define HEATER_LED_PIN                  LED1
#define LIGHT_LED_PIN                   LED2

// === Matrix keypad ===
// Keypad row outputs
#define KEYPAD_ROW_0_PIN  PE_4
#define KEYPAD_ROW_1_PIN  PE_5
#define KEYPAD_ROW_2_PIN  PE_6
#define KEYPAD_ROW_3_PIN  PE_3
// Keypad column inputs
#define KEYPAD_COL_0_PIN  PF_8
#define KEYPAD_COL_1_PIN  PF_7
#define KEYPAD_COL_2_PIN  PF_9
#define KEYPAD_COL_3_PIN  PG_1

// Display
#define DISPLAY_MOSI_PIN        PB_5
#define DISPLAY_MISO_PIN        PB_4
#define DISPLAY_SCK_PIN         PB_3
#define DISPLAY_CS_PIN  PA_15

#define DISPLAY_DC_PIN  PB_6
#define DISPLAY_RST_PIN PB_8

// SD card-specific
#define SD_MOSI_PIN        PE_14
#define SD_MISO_PIN        PE_13
#define SD_SCK_PIN         PE_12
#define SD_CS_PIN          PD_11 

#endif
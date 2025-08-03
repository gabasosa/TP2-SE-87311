#ifndef _PIN_MAP_H_
#define _PIN_MAP_H_

// === Sensors ===
#define TEMP_HUM_SENSOR_PIN PF_14

// === Actuators ===
#define HUMIDIFIER_CONTROL_PIN          PF_13
#define HUMIDIFIER_LED_INDICATOR_PIN    LED2
#define FAN_RELAY_PIN                   PF_12
#define FAN_LED_INDICATOR               LED3
#define SERVO_FLAP_PIN                  PD_15  // PWM4

#define PELTIER_RPWM_PIN        PE_9   // PWM1
#define PELTIER_LPWM_PIN        PA_0   // PWM2/1
#define PELTIER_REN_PIN         PE_0
#define PELTIER_LEN_PIN         PB_11
#define PELTIER_FAN1_CTRL_PIN   PB_10
#define PELTIER_FAN2_CTRL_PIN   PE_15

// === LEDs ===
#define HEATER_LED_PIN  LED1
#define LIGHT_LED_PIN   PE_11

// === Matrix keypad ===
// Keypad row outputs
#define KEYPAD_ROW_1_PIN  PE_4
#define KEYPAD_ROW_2_PIN  PE_5
#define KEYPAD_ROW_3_PIN  PE_6
#define KEYPAD_ROW_4_PIN  PE_3
// Keypad column inputs
#define KEYPAD_COL_1_PIN  PF_8
#define KEYPAD_COL_2_PIN  PF_7
#define KEYPAD_COL_3_PIN  PF_9
#define KEYPAD_COL_4_PIN  PG_1

// Display
#define DISPLAY_MOSI_PIN    PB_5
#define DISPLAY_MISO_PIN    PB_4
#define DISPLAY_SCK_PIN     PB_3
#define DISPLAY_CS_PIN      PA_15

#define DISPLAY_DC_PIN      PB_6
#define DISPLAY_RST_PIN     PB_8

// SD card-specific
#define SD_MOSI_PIN PE_14
#define SD_MISO_PIN PE_13
#define SD_SCK_PIN  PE_12
#define SD_CS_PIN   PD_11

// Touchscreen
#define TOUCH_CLK_PIN   PC_10  // SPI3_SCK
#define TOUCH_CS_PIN    PC_11  // SPI3_MISO as GPIO
#define TOUCH_DIN_PIN   PC_12  // SPI3_MOSI
#define TOUCH_DOUT_PIN  PD_2   // Input from touch
#define TOUCH_IRQ_PIN   PC_9   // Optional IRQ

// BLE
#define BLE_UART_TX_PIN PG_9
#define BLE_UART_RX_PIN PG_14 

#endif
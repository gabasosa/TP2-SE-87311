#include "cooler.h"
#include "mbed.h"
#include "config/pin_map.h"

static DigitalOut coolerControl(COOLER_PWM_PIN);
static DigitalOut coolerControlLEDIndicator(COOLER_PWM_LED_INDICATOR);

void Cooler_Init() {
    coolerControl = 0;  // Off by default
    coolerControlLEDIndicator = 0;
}

void Cooler_On() {
    coolerControl = 1;
    coolerControlLEDIndicator = 1;
}

void Cooler_Off() {
    coolerControl = 0;
    coolerControlLEDIndicator = 0;
}
#include "fan.h"
#include "mbed.h"
#include "config/pin_map.h"

static DigitalOut fanControl(FAN_RELAY_PIN);
static DigitalOut fanLEDIndicator(FAN_LED_INDICATOR);

void Fan_Init() {
    fanControl = 0;  // Off by default
    fanLEDIndicator = 0;
}

void Fan_On() {
    fanControl = 1;
    fanLEDIndicator = 1;
}

void Fan_Off() {
    fanControl = 0;
    fanLEDIndicator = 0;
}
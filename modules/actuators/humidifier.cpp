#include "humidifier.h"
#include "mbed.h"
#include "config/pin_map.h"

static DigitalOut humidifierRelay(HUMIDIFIER_RELAY_PIN);
static DigitalOut humidifierLEDIndicador(HUMIDIFIER_LED_INDICATOR);

void Humidifier_Init() {
    humidifierRelay = 0;  // Off by default
    humidifierLEDIndicador = 0;
}

void Humidifier_On() {
    humidifierRelay = 1;
    humidifierLEDIndicador = 1;
}

void Humidifier_Off() {
    humidifierRelay = 0;
    humidifierLEDIndicador = 0;
}
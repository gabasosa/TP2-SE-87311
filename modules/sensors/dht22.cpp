#include "sensors/dht22.h"
#include "sensors/DHT/DHT.h"
#include "config/pin_map.h"
#include "mbed.h"


static DHT dht(TEMP_HUM_SENSOR_PIN, DHT22);
static float lastTemperature = 0.0f;
static float lastHumidity = 0.0f;

static Ticker dhtTicker;
static volatile bool readFlag = false;

static void onTick() {
    readFlag = true;
}

void DHT22_Init() {
    dhtTicker.attach(&onTick, 2s);  // Handle timing internally
}

float DHT22_GetTemperature() {
    if (readFlag) {
        readFlag = false;
        if (dht.readData() == 0) {
            lastTemperature = dht.ReadTemperature();
            lastHumidity = dht.ReadHumidity();
        }
    }
    return lastTemperature;
}

float DHT22_GetHumidity() {
    return lastHumidity;
}
#ifndef _DHT22_SENSOR_H_
#define _DHT22_SENSOR_H_

#include "sensors/DHT/DHT.h"
#include "mbed.h"

#define TICKER_PERIOD_S   2

class DHT22Sensor {
public:
    DHT22Sensor(PinName pin);
    
    void begin();
    float getTemperature();
    float getHumidity();

private:
    void handleTick();

    DHT _dht;
    float _lastTemperature;
    float _lastHumidity;
    Ticker _ticker;
    volatile bool _readFlag;
};

#endif
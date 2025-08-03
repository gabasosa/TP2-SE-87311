#include "sensors/DHT22Sensor.h"

DHT22Sensor::DHT22Sensor(PinName pin)
    : _dht(pin, DHT22), _lastTemperature(0.0f), _lastHumidity(0.0f), _readFlag(false) {}

void DHT22Sensor::begin() {
    _ticker.attach(callback(this, &DHT22Sensor::handleTick), TICKER_PERIOD_S);
}

void DHT22Sensor::handleTick() {
    _readFlag = true;
}

float DHT22Sensor::getTemperature() {
    if (_readFlag) {
        _readFlag = false;
        if (_dht.readData() == 0) {
            _lastTemperature = _dht.ReadTemperature();
            _lastHumidity = _dht.ReadHumidity();
        }
    }
    return _lastTemperature;
}

float DHT22Sensor::getHumidity() {
    return _lastHumidity;
}
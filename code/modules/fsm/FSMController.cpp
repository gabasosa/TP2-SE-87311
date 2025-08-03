#include "FSMController.h"
#include "state_defs.h"
#include "time_utils.h"

FSMController::FSMController(DHT22Sensor& dhtRef, PeltierController& peltierRef, Fan& fanRef,
                             Humidifier& humidifierRef, Flap& flapRef)
    : _state(IDLE), _lastTemp(0.0f), _lastHumidity(0.0f),
      _lastStateChangeTime(0), _forceVentilation(false),
      _tempUpperThreshold(TEMP_UPPER_THRESHOLD_C),
      _tempLowerThreshold(TEMP_LOWER_THRESHOLD_C),
      _humLowerThreshold(HUMIDITY_LOWER_THRESHOLD),
      _coolingStartTime(0), _heatingStartTime(0), _humidifyingStartTime(0),
      _coolerLastUseTime(0), _heaterLastUseTime(0), _humidifierLastUseTime(0),
      _dht(dhtRef), _peltier(peltierRef), _fan(fanRef),
      _humidifier(humidifierRef), _flap(flapRef)
{}

void FSMController::begin() {
    _state = IDLE;
    _lastStateChangeTime = getTimeNow();
    _humidifier.off();
    _peltier.off();
    _fan.off();
    _flap.begin();
}

void FSMController::update() {
    uint32_t now = getTimeNow();

    if (_state == IDLE && (now - _lastStateChangeTime < STABILIZATION_DELAY_MS)) {
        return;
    }

    _lastTemp = _dht.getTemperature();
    _lastHumidity = _dht.getHumidity();

    switch (_state) {
        case IDLE:
            if (_forceVentilation || isVentilationTime(now)) {
                _flap.on();
                _fan.on();
                _state = VENTILATING;
                _lastStateChangeTime = now;
                _forceVentilation = false;
            } else if (_lastTemp >= _tempUpperThreshold &&
                       (now - _coolerLastUseTime) >= MIN_TIME_BETWEEN_COOLER_OPERATIONS_MS) {
                _peltier.setMode(PeltierController::COOLING);
                _peltier.setDutyCycle(1.0f);
                _peltier.on();
                _coolingStartTime = now;
                _state = COOLING;
                _lastStateChangeTime = now;
            } else if (_lastTemp <= _tempLowerThreshold &&
                       (now - _heaterLastUseTime) >= MIN_TIME_BETWEEN_HEATER_OPERATIONS_MS) {
                _peltier.setMode(PeltierController::HEATING);
                _peltier.setDutyCycle(1.0f);
                _peltier.on();
                _heatingStartTime = now;
                _state = HEATING;
                _lastStateChangeTime = now;
            } else if (_lastHumidity < _humLowerThreshold &&
                       (now - _humidifierLastUseTime) >= MIN_TIME_BETWEEN_HUMIDIFIER_OPERATIONS_MS) {
                _humidifier.on();
                _humidifyingStartTime = now;
                _state = HUMIDIFYING;
                _lastStateChangeTime = now;
            }
            break;

        case VENTILATING:
            if (now - _lastStateChangeTime >= VENTILATION_DURATION_MS) {
                _fan.off();
                _flap.off();
                _state = IDLE;
                _lastStateChangeTime = now;
            }
            break;

        case COOLING:
            if (_dht.getTemperature() < getTempUpperThreshold() ||
                now - _coolingStartTime >= MAX_COOLER_OPERATION_TIME_MS) {
                _peltier.off();
                _coolerLastUseTime = now;
                _state = IDLE;
                _lastStateChangeTime = now;
            }
            break;

        case HEATING:
            if (_dht.getTemperature() > getTempLowerThreshold() ||
                now - _heatingStartTime >= MAX_HEATER_OPERATION_TIME_MS) {
                _peltier.off();
                _heaterLastUseTime = now;
                _state = IDLE;
                _lastStateChangeTime = now;
            }
            break;

        case HUMIDIFYING:
            if (_dht.getHumidity() > getHumLowerThreshold() ||
                now - _humidifyingStartTime >= MAX_HUMIDIFIER_OPERATION_TIME_MS) {
                _humidifier.off();
                _humidifierLastUseTime = now;
                _state = IDLE;
                _lastStateChangeTime = now;
            }
            break;
    }
}

void FSMController::forceVentilation() {
    _forceVentilation = true;
}

const char* FSMController::getStateString() const {
    switch (_state) {
        case IDLE: return "IDLE";
        case COOLING: return "COOLING";
        case HEATING: return "HEATING";
        case HUMIDIFYING: return "HUMIDIFYING";
        case VENTILATING: return "VENTILATING";
        default: return "UNKNOWN";
    }
}

float FSMController::getLastTemp() const {
    return _lastTemp;
}

float FSMController::getLastHumidity() const {
    return _lastHumidity;
}

void FSMController::setHumLowerThreshold(float threshold) {
    _humLowerThreshold = threshold;
}

void FSMController::setTempLowerThreshold(float threshold) {
    _tempLowerThreshold = threshold;
}

void FSMController::setTempUpperThreshold(float threshold) {
    _tempUpperThreshold = threshold;
}

float FSMController::getHumLowerThreshold() const {
    return _humLowerThreshold;
}

float FSMController::getTempLowerThreshold() const {
    return _tempLowerThreshold;
}

float FSMController::getTempUpperThreshold() const {
    return _tempUpperThreshold;
}
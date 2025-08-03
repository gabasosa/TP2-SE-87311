#ifndef _FSM_CONTROLLER_H_
#define _FSM_CONTROLLER_H_

#include "sensors/DHT22Sensor.h"
#include "actuators/PeltierController.h"
#include "actuators/Fan.h"
#include "actuators/Humidifier.h"
#include "actuators/Flap.h"
#include "fsm/state_defs.h"
#include "mbed.h"

class FSMController {
public:
    FSMController(DHT22Sensor& dhtRef, PeltierController& peltierRef, Fan& fanRef,
                  Humidifier& humidifierRef, Flap& flapRef);

    void begin();
    void update();
    void forceVentilation();

    void setHumLowerThreshold(float threshold);
    void setTempLowerThreshold(float threshold);
    void setTempUpperThreshold(float threshold);

    float getHumLowerThreshold() const;
    float getTempLowerThreshold() const;
    float getTempUpperThreshold() const;

    const char* getStateString() const;
    float getLastTemp() const;
    float getLastHumidity() const;

private:
    // FSM state and thresholds
    MushroomerState_t _state;
    float _lastTemp;
    float _lastHumidity;
    uint32_t _lastStateChangeTime;
    float _tempLowerThreshold;
    float _tempUpperThreshold;
    float _humLowerThreshold;
    volatile bool _forceVentilation;

    // Internal timers and cooldown trackers
    uint32_t _coolingStartTime;
    uint32_t _heatingStartTime;
    uint32_t _humidifyingStartTime;
    uint32_t _coolerLastUseTime;
    uint32_t _heaterLastUseTime;
    uint32_t _humidifierLastUseTime;

    // References to external modules
    DHT22Sensor& _dht;
    PeltierController& _peltier;
    Fan& _fan;
    Humidifier& _humidifier;
    Flap& _flap;
};

#endif
#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "mbed.h"
#include "actuators/Actuator.h"

class Lights : public Actuator {
public:
    explicit Lights(PinName pin);

    void begin() override;
    void on() override;
    void off() override;
    bool isOn() override;
    void toggle();

private:
    DigitalOut _light;
};

#endif // _LIGHTS_H_
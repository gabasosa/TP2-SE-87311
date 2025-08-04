#ifndef _PELTIER_H_
#define _PELTIER_H_

#include "mbed.h"
#include "Actuator.h"

class PeltierController : public Actuator {
public:
    enum Mode { COOLING, HEATING };

    PeltierController(PinName rpwmPin, PinName lpwmPin,
            PinName renPin, PinName lenPin,
            PinName fan1Pin, PinName fan2Pin);

    void begin() override;
    void on(float dutyCycle);
    void on() override;
    void off() override;
    bool isOn() override;

    void setMode(Mode mode);
    void setDutyCycle(float duty);

private:
    Mode _mode;
    float _dutyCycle;
    bool _active;

    PwmOut _rpwm;
    PwmOut _lpwm;
    DigitalOut _ren;
    DigitalOut _len;
    DigitalOut _fan1;
    DigitalOut _fan2;
};

#endif
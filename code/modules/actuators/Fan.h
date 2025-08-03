#ifndef _FAN_H_
#define _FAN_H_

#include "mbed.h"
#include "Actuator.h"

class Fan : public Actuator {
public:
    Fan(PinName controlPin, PinName ledPin);
    void begin() override;
    void on() override;
    void off() override;
    bool isOn() override;

private:
    DigitalOut _control;
    DigitalOut _led;
};

#endif
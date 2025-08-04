#ifndef _HUMIDIFIER_H_
#define _HUMIDIFIER_H_

#include "mbed.h"
#include "Actuator.h"

class Humidifier : public Actuator {
public:
    Humidifier(PinName relayPin, PinName ledPin);
    void begin() override;
    void on() override;
    void off() override;
    bool isOn() override;

private:
    DigitalOut _relay;
    DigitalOut _led;
};

#endif
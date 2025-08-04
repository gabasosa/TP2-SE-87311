#ifndef _FLAP_H_
#define _FLAP_H_

#include "mbed.h"
#include "Actuator.h"

#define PWM_PERIOD_MS           20      // 50 Hz
#define PWM_OPENED_PERIOD_US      2500    // To open the flap (180°)
#define PWM_CLOSED_PERIOD_US    500     // To close the flap (0°)

class Flap : public Actuator {
public:
    Flap(PinName servoPin);
    void begin() override;
    void on() override;     // Open
    void off() override;    // Close
    bool isOn() override;

private:
    PwmOut _servo;
    bool _isOpen;
};

#endif
#include "Flap.h"

Flap::Flap(PinName servoPin)
    : _servo(servoPin), _isOpen(false) {}

void Flap::begin() {
    _servo.period_ms(PWM_PERIOD_MS);  // 50 Hz servo signal
    off();                 // Default to closed
}

void Flap::on() {
    _servo.pulsewidth_us(PWM_OPENED_PERIOD_US);
    _isOpen = true;
}

void Flap::off() {
    _servo.pulsewidth_us(PWM_CLOSED_PERIOD_US);
    _isOpen = false;
}

bool Flap::isOn() {
    return _isOpen;
}
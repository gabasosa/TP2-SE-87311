#include "Fan.h"
#include "arm_book_lib.h"

Fan::Fan(PinName controlPin, PinName ledPin)
    : _control(controlPin), _led(ledPin) {}

void Fan::begin() {
    _control = OFF;
    _led = OFF;
}

void Fan::on() {
    _control = ON;
    _led = ON;
}

void Fan::off() {
    _control = OFF;
    _led = OFF;
}

bool Fan::isOn() {
    return _control.read();
}
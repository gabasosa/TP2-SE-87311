#include "lights/Lights.h"
#include "arm_book_lib.h"

Lights::Lights(PinName pin) : _light(pin) {
    off();
}

void Lights::begin() {
    // No initialization needed
}

void Lights::on() {
    _light = ON;
}

void Lights::off() {
    _light = OFF;
}

bool Lights::isOn() {
    return _light == ON;
}

void Lights::toggle() {
    if (isOn()) {
        off();
    } else {
        on();
    }
}
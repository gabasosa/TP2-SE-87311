#include "Humidifier.h"
#include "arm_book_lib.h"

Humidifier::Humidifier(PinName relayPin, PinName ledPin)
    : _relay(relayPin), _led(ledPin) {}

void Humidifier::begin() {
    off();
}

void Humidifier::on() {
    _relay = ON;
    _led = ON;
}

void Humidifier::off() {
    _relay = OFF;
    _led = OFF;
}

bool Humidifier::isOn() {
    return _relay.read();
}
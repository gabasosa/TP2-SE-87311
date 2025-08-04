#include "PeltierController.h"
#include "arm_book_lib.h"

PeltierController::PeltierController(PinName rpwmPin, PinName lpwmPin,
                 PinName renPin, PinName lenPin,
                 PinName fan1Pin, PinName fan2Pin)
    : _rpwm(rpwmPin), _lpwm(lpwmPin),
      _ren(renPin), _len(lenPin),
      _fan1(fan1Pin), _fan2(fan2Pin),
      _mode(COOLING), _dutyCycle(1.0f), _active(false) {}

void PeltierController::begin() {
    _rpwm.period_us(40);  // 25 kHz
    _lpwm.period_us(40);
    off();
}

void PeltierController::setMode(Mode mode) {
    _mode = mode;
}

void PeltierController::setDutyCycle(float duty) {
    _dutyCycle = (duty < 0.0f) ? 0.0f : ((duty > 1.0f) ? 1.0f : duty);
}

void PeltierController::on() {
    on(_dutyCycle);
}

void PeltierController::on(float dutyCycle) {
    if (_mode == COOLING) {
        _rpwm.write(0.0f);
        _lpwm.write(_dutyCycle);
    } else { // HEATING
        _lpwm.write(0.0f);
        _rpwm.write(_dutyCycle);
    }

    _ren = ON;
    _len = ON;
    _fan1 = ON;
    _fan2 = ON;
    _active = true;
}

void PeltierController::off() {
    _rpwm.write(0.0f);
    _lpwm.write(0.0f);
    _ren = OFF;
    _len = OFF;
    _fan1 = OFF;
    _fan2 = OFF;
    _active = false;
}

bool PeltierController::isOn() {
    return _active;
}
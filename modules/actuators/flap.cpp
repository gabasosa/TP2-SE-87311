#include "mbed.h"
#include "flap.h"
#include "config/pin_map.h"

static PwmOut flapServo(SERVO_FLAP_PIN);

void Flap_Init() {
    flapServo.period_ms(20);  // 50 Hz
    Flap_Close();             // Default to closed position
}

void Flap_Open() {
    flapServo.pulsewidth_us(2500);  // 180°
}

void Flap_Close() {
    flapServo.pulsewidth_us(500);   // 00°
}
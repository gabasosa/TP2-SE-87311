#ifndef _STATE_DEFS_H_
#define _STATE_DEFS_H_

#include "mbed.h"

//=== States ==================================================

typedef enum {
    IDLE,
    HUMIDIFYING,
    COOLING,
    HEATING,
    VENTILATING
} MushroomerState_t;

//=== Constants ================================================

// Thresholds
#define HUMIDITY_LOWER_THRESHOLD    50.0f
#define TEMP_LOWER_THRESHOLD_C      20.0f
#define TEMP_UPPER_THRESHOLD_C      30.0f


// Timings (in ms)
#define MAX_HUMIDIFIER_OPERATION_TIME_MS            300000
#define MIN_TIME_BETWEEN_HUMIDIFIER_OPERATIONS_MS 10000
#define MAX_COOLER_OPERATION_TIME_MS    300000
#define MIN_TIME_BETWEEN_COOLER_OPERATIONS_MS 10000
#define MAX_HEATER_OPERATION_TIME_MS    300000
#define MIN_TIME_BETWEEN_HEATER_OPERATIONS_MS 10000
#define VENTILATION_DURATION_MS         30000
#define VENTILATION_INTERVAL_MS         900000
#define STABILIZATION_DELAY_MS          2000
#define FSM_UPDATE_PERIOD_MS            100    // main FSM loop period


// Peltier PWM power control
#define PELTIER_MAX_POWER_DUTY_CYCLE    1.0f

#endif
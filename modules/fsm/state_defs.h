#ifndef _STATE_DEFS_H_
#define _STATE_DEFS_H_

#include "mbed.h"

//=== States ==================================================

typedef enum {
    STAND_BY,
    HUMIDIFYING,
    COOLING,
    VENTILATING
} MushroomerState_t;

//=== Constants ================================================

// Thresholds
#define TEMP_UPPER_THRESHOLD_C      28.0
#define TEMP_LOWER_THRESHOLD_C      20.0
#define HUMIDITY_LOWER_THRESHOLD    85.0f  // for DHT22

// Timings (in ms)
#define HUMIDIFY_DURATION_MS        10000
#define COOLING_DURATION_MS         10000
#define VENTILATION_DURATION_MS     5000
#define VENTILATION_INTERVAL_MS     60000
#define STABILIZATION_DELAY_MS      5000
#define FSM_UPDATE_PERIOD_MS        100    // main FSM loop period

#endif
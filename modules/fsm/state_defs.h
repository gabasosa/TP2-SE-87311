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
#define TEMP_UPPER_THRESHOLD_C      15.0
#define TEMP_LOWER_THRESHOLD_C      20.0
#define HUMIDITY_LOWER_THRESHOLD    95.0f  // for DHT22

// Timings (in ms)
#define MAX_HUMIDIFIER_OPERATION_TIME_MS            20000
#define MIN_TIME_BETWEEN_HUMIDIFYING_OPERATIONS_MS 10000
#define MAX_COOLER_OPERATION_TIME_MS    20000
#define MIN_TIME_BETWEEN_COOLER_OPERATIONS_MS 10000 // 1 minute
#define VENTILATION_DURATION_MS         60000
#define VENTILATION_INTERVAL_MS         300000
#define STABILIZATION_DELAY_MS          2000
#define FSM_UPDATE_PERIOD_MS            100    // main FSM loop period


#endif
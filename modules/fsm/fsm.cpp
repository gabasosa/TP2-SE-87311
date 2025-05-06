#include "fsm.h"
#include "sensors/dht22.h"
#include "sensors/time_utils.h"
#include "actuators/humidifier.h"
#include "actuators/cooler.h"
#include "actuators/fan.h"

int tempThresholdUpper = 25; // Default upper temp in °C
int humThresholdLower = 60;  // Default lower humidity in %

//=== STATE MACHINE INTERNALS ===//

static MushroomerState_t currentState;
static uint32_t lastStateChangeTime = 0;
static float lastTempC = 0.0f;
static float lastHumidity = 0.0f;

void FSM_Init() {
    currentState = STAND_BY;
    lastStateChangeTime = getTimeNow();
    Humidifier_Off();
    Cooler_Off();
    Fan_Off();
}

MushroomerState_t FSM_GetState() {
    return currentState;
}

float FSM_GetLastTemp() {
    return lastTempC;
}

float FSM_GetLastHumidity() {
    return lastHumidity;
}

void FSM_Update() {
    uint32_t now = getTimeNow();

    // Wait after state change before sensing again
    if (currentState == STAND_BY && (now - lastStateChangeTime < STABILIZATION_DELAY_MS)) {
        return;
    }

    // Update latest sensor readings
    lastTempC = DHT22_GetTemperature();
    lastHumidity = DHT22_GetHumidity();

    switch (currentState) {

    case STAND_BY:
        if (lastHumidity < humThresholdLower) {
            Humidifier_On();
            currentState = HUMIDIFYING;
        }
        else if (lastTempC > tempThresholdUpper) {
            Cooler_On();
            currentState = COOLING;
        }
        else if (isVentilationTime(now)) {
            Fan_On();
            currentState = VENTILATING;
        }

        if (currentState != STAND_BY)
            lastStateChangeTime = now;
        break;

    case HUMIDIFYING:
        if (now - lastStateChangeTime >= HUMIDIFY_DURATION_MS) {
            Humidifier_Off();
            currentState = STAND_BY;
            lastStateChangeTime = now;
        }
        break;

    case COOLING:
        if (now - lastStateChangeTime >= COOLING_DURATION_MS) {
            Cooler_Off();
            currentState = STAND_BY;
            lastStateChangeTime = now;
        }
        break;

    case VENTILATING:
        if (now - lastStateChangeTime >= VENTILATION_DURATION_MS) {
            Fan_Off();
            currentState = STAND_BY;
            lastStateChangeTime = now;
        }
        break;
    }
}

const char* FSM_StateToString(MushroomerState_t state) {
    switch (state) {
        case STAND_BY:
            return "STAND_BY";
        case HUMIDIFYING:
            return "HUMIDIFYING";
        case COOLING:
            return "COOLING";
        case VENTILATING:
            return "VENTILATING";
        default:
            return "UNKNOWN";
    }
}
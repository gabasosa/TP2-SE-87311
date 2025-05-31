#include "fsm/fsm.h"
#include "sensors/dht22.h"
#include "sensors/time_utils.h"
#include "actuators/flap.h"
#include "actuators/humidifier.h"
#include "actuators/cooler.h"
#include "actuators/fan.h"


//=== GLOBAL VARIABLES ===//
int tempThresholdUpper = 25; // Default upper temp in °C
int humThresholdLower = 60;  // Default lower humidity in %
static uint32_t coolerLastUseTime = 0;
static uint32_t coolingStartTime = 0;
static uint32_t humidifierLastUseTime = 0;
static uint32_t humidifyingStartTime = 0;
extern volatile bool forceVentilation;
extern volatile char key;

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
    Flap_Init();
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
    static bool coolingJustEnded = false;       // Flag to defer lockout update
    static bool humidifyingJustEnded = false;
    uint32_t now = getTimeNow();

    // Update cooler lockout timestamp (after cooldown ends)
    if (coolingJustEnded) {
        coolerLastUseTime = now;
        coolingJustEnded = false;
    }

    if (humidifyingJustEnded) {
        humidifierLastUseTime = now;
        humidifyingJustEnded = false;
    }

    // Wait after state change before sensing again
    if (currentState == STAND_BY && (now - lastStateChangeTime < STABILIZATION_DELAY_MS)) {
        return;
    }

    // Sensor reads only after stabilization
    lastTempC = DHT22_GetTemperature();
    lastHumidity = DHT22_GetHumidity();

    switch (currentState) {

    case STAND_BY:
        if (forceVentilation || isVentilationTime(now)) {
            Flap_Open();
            Fan_On();
            currentState = VENTILATING;
            lastStateChangeTime = now;
            forceVentilation = false;
            break;
        }

        if (lastTempC >= tempThresholdUpper &&
            (now - coolerLastUseTime) >= MIN_TIME_BETWEEN_COOLER_OPERATIONS_MS)
        {
            Cooler_On();
            currentState = COOLING;
            coolingStartTime = now;
            lastStateChangeTime = now;
            break;
        }

        if (lastHumidity < humThresholdLower &&
            (now - humidifierLastUseTime) >= MIN_TIME_BETWEEN_HUMIDIFYING_OPERATIONS_MS)
        {
            Humidifier_On();
            currentState = HUMIDIFYING;
            humidifyingStartTime = now;
            lastStateChangeTime = now;
            break;
        }
        break;

    case COOLING:
        if (
            (now - coolingStartTime) > MAX_COOLER_OPERATION_TIME_MS ||  // Real elapsed time
            lastTempC < tempThresholdUpper
        ) {
            Cooler_Off();
            coolingJustEnded = true;
            currentState = STAND_BY;
            lastStateChangeTime = now;
        }
        break;

    case VENTILATING:
        if (now - lastStateChangeTime >= VENTILATION_DURATION_MS) {
            Fan_Off();
            Flap_Close();
            currentState = STAND_BY;
            lastStateChangeTime = now;
        }
        break;

    case HUMIDIFYING:
        if (now - humidifyingStartTime >= MAX_HUMIDIFIER_OPERATION_TIME_MS) {
            Humidifier_Off();
            humidifyingJustEnded = true;
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
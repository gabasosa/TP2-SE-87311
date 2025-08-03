#include <chrono>
#include "mbed.h"
#include "time_utils.h"
#include "fsm/state_defs.h"

using namespace std::chrono;

uint64_t getTimeNow() {
    return duration_cast<milliseconds>(Kernel::Clock::now().time_since_epoch()).count();
}

std::time_t getRtcTime() {
    return time(NULL);  // safe only in thread context
}

bool isVentilationTime(uint32_t now) {
    static uint32_t lastVentilationTimestamp = 0;

    if (now - lastVentilationTimestamp >= VENTILATION_INTERVAL_MS) {
        lastVentilationTimestamp = now;
        return true;
    }
    return false;
}
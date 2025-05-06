#ifndef _TIME_UTILS_H_
#define _TIME_UTILS_H_

#include "mbed.h"

// Safe for FSM/ISR use (monotonic milliseconds since boot)
uint64_t getTimeNow();

// Use only in thread context (calendar time from RTC)
std::time_t getRtcTime();

bool isVentilationTime(uint32_t now);

#endif
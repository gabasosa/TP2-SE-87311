#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include "mbed.h"
#include "fsm/fsm.h"

#define MAX_LOG_ENTRIES 20
#define MAX_LOG_LENGTH  128

typedef struct {
    time_t timestamp;
    MushroomerState_t state;
    float temperature;
    float humidity;
} EventLogEntry_t;

void EventLogger_Init();
void EventLogger_AddEntry(MushroomerState_t state, float temperature, float humidity);
void EventLogger_PrintAll();

// New functions:
int EventLogger_GetEntryCount();
EventLogEntry_t EventLogger_GetEntry(int index);

#endif // EVENT_LOGGER_H
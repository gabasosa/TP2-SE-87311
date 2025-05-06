#include "event_logger.h"
#include <cstdio>

static EventLogEntry_t logEntries[MAX_LOG_ENTRIES];
static int logIndex = 0;

void EventLogger_Init() {
    logIndex = 0;
}

void EventLogger_AddEntry(MushroomerState_t state, float temperature, float humidity) {
    if (logIndex < MAX_LOG_ENTRIES) {
        logEntries[logIndex].timestamp = time(NULL);
        logEntries[logIndex].state = state;
        logEntries[logIndex].temperature = temperature;
        logEntries[logIndex].humidity = humidity;
        logIndex++;
    }
    // Optionally: implement circular buffer if needed later
}

void EventLogger_PrintAll() {
    for (int i = 0; i < logIndex; ++i) {
        struct tm *timeinfo = localtime(&logEntries[i].timestamp);
        char timeStr[32];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

        printf("[%02d] %s | %s | Temp: %.2f ºC | Humidity: %.2f %%\r\n",
            i,
            timeStr,
            FSM_StateToString(logEntries[i].state),
            logEntries[i].temperature,
            logEntries[i].humidity
        );
    }
}

int EventLogger_GetEntryCount() {
    return logIndex;
}

EventLogEntry_t EventLogger_GetEntry(int index) {
    static EventLogEntry_t empty = {0};

    if (index < 0 || index >= logIndex) {
        return empty;
    }

    return logEntries[index];
}

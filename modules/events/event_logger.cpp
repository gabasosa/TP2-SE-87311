#include "event_logger.h"
#include "sdcard.h"
#include <cstdio>
#include <cstring>
#include <ctime>

#define CSV_HEADER "Timestamp,State,Temperature [°C],Humidity [%%]\n"

static EventLogEntry_t logEntries[MAX_LOG_ENTRIES];
static int logIndex = 0;

void EventLogger_Init() {
    logIndex = 0;
}

// Helper to check if header exists
static bool SDCard_HeaderMissing(const char* filepath) {
    FILE* fp = fopen(filepath, "r");
    if (fp == NULL) return true;  // File does not exist — needs header

    char buffer[128];
    bool headerMissing = true;

    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "Timestamp") != NULL) {
            headerMissing = false;
        }
    }

    fclose(fp);
    return headerMissing;
}

void EventLogger_AddEntry(MushroomerState_t state, float temperature, float humidity) {
    const char* filepath = "/sd/events.csv";

    // Store in memory
    if (logIndex < MAX_LOG_ENTRIES) {
        logEntries[logIndex].timestamp = time(NULL);
        logEntries[logIndex].state = state;
        logEntries[logIndex].temperature = temperature;
        logEntries[logIndex].humidity = humidity;
        logIndex++;
    }

    if (SDCard_IsMounted() == false) {
        printf("[ERROR] SD card not mounted!\n");
        return;
    }

    // Check header before appending
    bool headerNeeded = SDCard_HeaderMissing(filepath);

    FILE* fp = fopen(filepath, "a");
    if (fp == NULL) {
        printf("[ERROR] Could not open log file\n");
        return;
    }

    if (headerNeeded) {
        fprintf(fp, CSV_HEADER);
    }

    // Get timestamp
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timeStr[32];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Write entry
    fprintf(fp, "%s,%s,%.2f,%.2f\n",
        timeStr,
        FSM_StateToString(state),
        temperature,
        humidity
    );

    fclose(fp);
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
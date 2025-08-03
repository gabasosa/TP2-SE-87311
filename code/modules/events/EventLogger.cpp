#include "EventLogger.h"
#include <cstdio>
#include <cstring>
#include <ctime>
#include "sdcard/SDCardManager.h"

//=====[Defines and Constants]===============================================//

#define MAX_DISPLAY_LOGS 6

//=====[Class Implementation]=================================================//

EventLogger::EventLogger(SDCardManager& sdRef)
    : sd(sdRef), _displayLogCount(0) {
    strcpy(filepath, "/sd/log.txt");
    for (int i = 0; i < MAX_DISPLAY_LOGS; i++) {
        displayLogs[i][0] = '\0';
    }
}

void EventLogger::begin() {
    if (sd.isMounted()) {
        loadRecentEntries();
    }
}

bool EventLogger::addEntry(const char* stateStr, float temperature, float humidity) {
    if (!sd.isMounted()) return false;

    writeHeaderIfNeeded();

    FILE* fp = fopen(filepath, "a");
    if (fp != nullptr) {
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char timeStr[32];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

        char line[128];
        snprintf(line, sizeof(line), "%s,%s,%.2f,%.2f\n",
                 timeStr, stateStr, temperature, humidity);

        fputs(line, fp);
        fclose(fp);

        // Save to display buffer
        strncpy(displayLogs[_displayLogCount % MAX_DISPLAY_LOGS], line, sizeof(displayLogs[0]));
        displayLogs[_displayLogCount % MAX_DISPLAY_LOGS][sizeof(displayLogs[0]) - 1] = '\0';
        _displayLogCount++;

        return true;
    }

    return false;
}

void EventLogger::writeHeaderIfNeeded() {
    FILE* fp = fopen(filepath, "r");
    if (fp == nullptr) {
        fp = fopen(filepath, "w");
        if (fp != nullptr) {
            fputs("timestamp,state,temperature,humidity\n", fp);
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
}

void EventLogger::loadRecentEntries() {
    FILE* fp = fopen(filepath, "r");
    if (fp == nullptr) return;

    char line[128];
    int count = 0;
    char tempBuffer[MAX_DISPLAY_LOGS][128];

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "timestamp", 9) == 0) continue; // skip header
        strncpy(tempBuffer[count % MAX_DISPLAY_LOGS], line, sizeof(tempBuffer[0]));
        count++;
    }
    fclose(fp);

    int start = (count >= MAX_DISPLAY_LOGS) ? (count % MAX_DISPLAY_LOGS) : 0;
    int total = (count < MAX_DISPLAY_LOGS) ? count : MAX_DISPLAY_LOGS;

    for (int i = 0; i < total; i++) {
        int index = (start + i) % MAX_DISPLAY_LOGS;
        strncpy(displayLogs[i], tempBuffer[index], sizeof(displayLogs[i]));
        displayLogs[i][sizeof(displayLogs[i]) - 1] = '\0';
    }

    _displayLogCount = total;
}

int EventLogger::getEntryCount() const {
    return _displayLogCount;
}

const char* EventLogger::getEntry(int index) const {
    if (index < 0 || index >= _displayLogCount) return "";
    return displayLogs[index];
}

const char* EventLogger::getDisplayEntry(int relativeIndex, int entriesToShow) {
    static char line[128];

    if (!sd.isMounted()) {
        snprintf(line, sizeof(line), "SD not mounted");
        return line;
    }

    FILE* fp = fopen(filepath, "r");
    if (!fp) {
        snprintf(line, sizeof(line), "Error opening log");
        return line;
    }

    // Count total lines
    int totalLines = 0;
    while (fgets(line, sizeof(line), fp)) totalLines++;

    // Compute which absolute line to show based on relative index
    int startIndex = totalLines - entriesToShow;
    if (startIndex < 0) startIndex = 0;

    int targetLine = startIndex + relativeIndex;

    // Rewind and read until target line
    rewind(fp);
    int currentLine = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (currentLine++ == targetLine) {
            fclose(fp);
            return line;
        }
    }

    fclose(fp);
    snprintf(line, sizeof(line), "No entry at index %d", relativeIndex);
    return line;
}
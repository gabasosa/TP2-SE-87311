#ifndef _EVENT_LOGGER_H_
#define _EVENT_LOGGER_H_

#include "sdcard/SDCardManager.h"

#define MAX_DISPLAY_LOGS 6

class EventLogger {
public:
    EventLogger(SDCardManager& sdRef);

    void begin();
    bool addEntry(const char* stateStr, float temperature, float humidity);
    int getEntryCount() const;
    const char* getEntry(int index) const;
    const char* getDisplayEntry(int relativeIndex, int entriesToShow);

private:
    SDCardManager& sd;
    char filepath[64];
    char displayLogs[MAX_DISPLAY_LOGS][128];
    int _displayLogCount;

    void writeHeaderIfNeeded();
    void loadRecentEntries();
};

#endif
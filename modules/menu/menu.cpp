#include "menu.h"
#include "fsm.h"
#include "dht22.h"
#include "event_logger.h"
#include "keypad.h"
#include "sdcard.h"
#include "tft.h"
#include <cstdio>
#include <cstring>
#include <ctime>

volatile bool forceVentilation;
Ticker clockTicker;
volatile bool clockUpdateFlag = false;

static enum MenuState {
    MENU_MAIN,
    MENU_SET_RTC
} menuState;

static char rtcInputStr[15];
static int rtcInputIndex = 0;

void onClockTick() {
    clockUpdateFlag = true;
}

void Menu_Init() {
    tft_init();
    tft_clear(TFT_WHITE);
    menuState = MENU_MAIN;
    Menu_Show();

    clockTicker.attach(&onClockTick, 1s);
}

void Menu_DrawClock() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    char timeStr[9]; // "HH:MM:SS"
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", t);

    int x = 254;  // Adjust as needed to fit 8 characters on the screen
    int y = 230;
    tft_text(x, y, timeStr, TFT_GREEN, TFT_WHITE);
}

void Menu_Show() {
    char D_option[19];

    if (SDCard_IsMounted() == true) {
        strcpy(D_option, "D: Unmount SD card");
    }
    else {
        strcpy(D_option, "D: Mount SD card");
    }

    tft_clear(TFT_WHITE);
    tft_text(10, 10, (char*) ">>>------ Mushroomer Menu ------<<<", TFT_BLACK, TFT_WHITE);
    tft_text(10, 30, (char*) "1: FSM State", TFT_BLACK, TFT_WHITE);
    tft_text(10, 50, (char*) "2: Temp & Humidity", TFT_BLACK, TFT_WHITE);
    tft_text(10, 70, (char*) "3: Set RTC Time", TFT_BLACK, TFT_WHITE);
    tft_text(10, 90, (char*) "4: Show Current Time", TFT_BLACK, TFT_WHITE);
    tft_text(10, 110, (char*) "5: Save State to Log", TFT_BLACK, TFT_WHITE);
    tft_text(10, 130, (char*) "6: View Event Log", TFT_BLACK, TFT_WHITE);
    tft_text(10, 150, (char*) "A: Set upper temperature threshold", TFT_BLACK, TFT_WHITE);
    tft_text(10, 170, (char*) "B: Set lower humidity threshold", TFT_BLACK, TFT_WHITE);
    tft_text(10, 190, (char*) "C: Force ventilation", TFT_BLACK, TFT_WHITE);
    tft_text(10, 210, D_option, TFT_BLACK, TFT_WHITE);
    tft_text(10, 230, (char*) "#: Return to Menu", TFT_BLACK, TFT_WHITE);
    Menu_DrawClock(); 
}

// Helper to safely print to display
void Display_ResetAndPrint(const char* text) {
    tft_init();
    tft_clear(TFT_WHITE);  // White background
    tft_text(10, 20, (char*)text, TFT_BLACK, TFT_WHITE);  // Black text
    Menu_DrawClock(); 
}

void Menu_HandleKey(char key) {
    char goBack[31] = "Press # to go back to the menu";

    switch (menuState) {

    case MENU_MAIN:

        switch (key) {

        case '1': {
            Display_ResetAndPrint(">>>------ FSM State ------<<<");

            MushroomerState_t state = FSM_GetState();
            const char* stateStr = FSM_StateToString(state);

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "<> %s", stateStr);
            tft_text(10, 50, buffer, TFT_BLACK, TFT_WHITE);
            tft_text(10, 90, goBack, TFT_BLACK, TFT_WHITE);
            break;
        }

        case '2': {
            Display_ResetAndPrint(">>>------ Temp & Hum ------<<<");
            float temp = FSM_GetLastTemp();
            float hum = FSM_GetLastHumidity();

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "Temp: %.2fC (upper threshold %.2fC", temp, (float)tempThresholdUpper);
            tft_text(10, 50, buffer, TFT_BLACK, TFT_WHITE);
            snprintf(buffer, sizeof(buffer), "Hum: %.2f%% (lower threshold %.2f%%", hum, (float)humThresholdLower);
            tft_text(10, 70, buffer, TFT_BLACK, TFT_WHITE);
            tft_text(10, 110, goBack, TFT_BLACK, TFT_WHITE);
            break;
        }

        case '3': 
            Display_ResetAndPrint(">>>---- Setting RTC via keypad ----<<<");
            menuState = MENU_SET_RTC;
            break;

        case '4': {
            Display_ResetAndPrint(">>>---- Current Time ----<<<");
            time_t currentTime = time(NULL);
            struct tm* localTime = localtime(&currentTime);
            char buffer[32];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
            tft_text(10, 50, buffer, TFT_BLACK, TFT_WHITE);
            tft_text(10, 90, goBack, TFT_BLACK, TFT_WHITE);
            break;
        }

        case '5': {
            Display_ResetAndPrint(">>>------ Event Log ------<<<");
            MushroomerState_t state = FSM_GetState();
            float temp = FSM_GetLastTemp();
            float hum = FSM_GetLastHumidity();
            EventLogger_AddEntry(state, temp, hum);

            tft_text(10, 50, (char*)"Entry saved to log", TFT_BLACK, TFT_WHITE);
            tft_text(10, 90, goBack, TFT_BLACK, TFT_WHITE);
            break;
        }

        case '6': {
            Display_ResetAndPrint(">>>------ Event Log ------<<<");

            int totalEntries = EventLogger_GetEntryCount();
            if (totalEntries == 0) {
                Display_ResetAndPrint(">>>------ Event Log ------<<<");
                tft_text(10, 50, (char*)"No entries yet", TFT_BLACK, TFT_WHITE);
                tft_text(10, 90, goBack, TFT_BLACK, TFT_WHITE);
                break;
            }

            int entriesToShow = (totalEntries >= 5) ? 5 : totalEntries;
            int startIndex = totalEntries - entriesToShow;

            for (int i = 0; i < entriesToShow; i++) {
                EventLogEntry_t entry = EventLogger_GetEntry(startIndex + i);

                // Format time and entry data
                struct tm* timeinfo = localtime(&entry.timestamp);
                char line1[32];
                char line2[64];

                strftime(line1, sizeof(line1), "%Y-%m-%d %H:%M:%S", timeinfo);
                snprintf(line2, sizeof(line2), "%s T=%.1fC H=%.1f%%",
                        FSM_StateToString(entry.state), entry.temperature, entry.humidity);

                int yBase = 50 + i * 40;
                tft_text(10, yBase,     line1, TFT_BLACK, TFT_WHITE);
                tft_text(10, yBase + 15, line2, TFT_BLACK, TFT_WHITE);
            }

            break;
        }

        case 'A': {
            char str[3];
            int strIndex = 0;

            Display_ResetAndPrint("Set max temp (e.g. 25):");

            while (strIndex < 2) {
                char key = matrixKeypadUpdate();
                if (key != '\0') {
                    str[strIndex++] = key;
                    str[strIndex] = '\0';

                    char chStr[2] = { key, '\0' };
                    tft_text(10 + (strIndex - 1) * 10, 80, chStr, TFT_BLACK, TFT_WHITE);
                }
            }

            tempThresholdUpper = atoi(str);
            char msg[32];
            snprintf(msg, sizeof(msg), "Max temp set: %d C", tempThresholdUpper);
            Display_ResetAndPrint(msg);
            break;
        }

        case 'B': {
            char str[3];
            int strIndex = 0;

            Display_ResetAndPrint("Set min hum (e.g. 40):");

            while (strIndex < 2) {
                char key = matrixKeypadUpdate();
                if (key != '\0') {
                    str[strIndex++] = key;
                    str[strIndex] = '\0';

                    char chStr[2] = { key, '\0' };
                    tft_text(10 + (strIndex - 1) * 10, 80, chStr, TFT_BLACK, TFT_WHITE);
                }
            }

            humThresholdLower = atoi(str);
            char msg[32];
            snprintf(msg, sizeof(msg), "Min hum set: %d%%", humThresholdLower);
            Display_ResetAndPrint(msg);
            break;
        }

        case 'C': {
            Display_ResetAndPrint(">>> Force ventilation requested <<<");
            forceVentilation = true;
            tft_text(10, 50, (char*)"Fan will turn on soon", TFT_BLACK, TFT_WHITE);
            tft_text(10, 90, goBack, TFT_BLACK, TFT_WHITE);
            break;
        }

        case 'D': {
            Display_ResetAndPrint(">>> SD Card Action <<<");

            if (SDCard_IsMounted()) {
                bool success = SDCard_SafeRemove();
                if (success) {
                    tft_text(10, 50, (char*)"The SD card can be safely removed now", TFT_BLACK, TFT_WHITE);
                } else {
                    tft_text(10, 50, (char*)"Error removing SD card", TFT_RED, TFT_WHITE);
                }
            } else {
                bool success = SDCard_Init();
                if (success) {
                    tft_text(10, 50, (char*)"SD card mounted OK", TFT_GREEN, TFT_WHITE);
                } else {
                    tft_text(10, 50, (char*)"Mount failed (is it on its place?)", TFT_RED, TFT_WHITE);
                }
            }

            tft_text(10, 90, goBack, TFT_BLACK, TFT_WHITE);
            break;
        }

        case '#':
            Display_ResetAndPrint(NULL);
            Menu_Show();
            break;

        default: {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "Invalid key: '%c'", key);
            tft_text(10, 50, buffer, TFT_BLACK, TFT_WHITE);
            tft_text(10, 70, goBack, TFT_BLACK, TFT_WHITE);
        }
        }
        break;

    case MENU_SET_RTC:
    if (key >= '0' && key <= '9' && rtcInputIndex < 14) {
        rtcInputStr[rtcInputIndex++] = key;
        rtcInputStr[rtcInputIndex] = '\0';

        tft_init();
        tft_clear(TFT_WHITE);
        tft_text(10, 20, (char*)"Set RTC Time:", TFT_BLACK, TFT_WHITE);
        tft_text(10, 40, (char*)"YYYYMMDDHHMMSS", TFT_GRAY, TFT_WHITE);
        tft_text(10, 60, rtcInputStr, TFT_ORANGE, TFT_WHITE);
    }

    if (rtcInputIndex == 14) {
        struct tm rtcTime;

        char temp[5];
        memcpy(temp, rtcInputStr, 4); temp[4] = '\0';
        rtcTime.tm_year = atoi(temp) - 1900;

        memcpy(temp, rtcInputStr + 4, 2); temp[2] = '\0';
        rtcTime.tm_mon = atoi(temp) - 1;

        memcpy(temp, rtcInputStr + 6, 2); temp[2] = '\0';
        rtcTime.tm_mday = atoi(temp);

        memcpy(temp, rtcInputStr + 8, 2); temp[2] = '\0';
        rtcTime.tm_hour = atoi(temp);

        memcpy(temp, rtcInputStr + 10, 2); temp[2] = '\0';
        rtcTime.tm_min = atoi(temp);

        memcpy(temp, rtcInputStr + 12, 2); temp[2] = '\0';
        rtcTime.tm_sec = atoi(temp);

        rtcTime.tm_isdst = -1;
        set_time(mktime(&rtcTime));

        tft_clear(TFT_WHITE);
        tft_text(10, 40, (char*)"RTC successfully set!", TFT_GREEN, TFT_WHITE);

        thread_sleep_for(1500);
        tft_init();
        tft_clear(TFT_WHITE);
        Menu_Show();
        menuState = MENU_MAIN;
    }
    break;

    default:
        break;
    }
}
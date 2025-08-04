#include "Menu.h"
#include "KeypadScanner.h"
#include "SDCardManager.h"
#include "EventLogger.h"
#include "FSMController.h"

#include "arm_book_lib.h"
#include "tft.h"
#include "menu_layout.h"
#include <cstring>
#include <ctime>
#include <cstdio>

static Ticker clockTicker;
volatile bool clockUpdateFlag = false;

Menu::Menu(FSMController& fsm, EventLogger& logger, KeypadScanner& keypad, SDCardManager& sdManager)
    : _fsm(fsm), _logger(logger), _keypad(keypad), _sdManager(sdManager), menuState(MENU_MAIN), _rtcInputIndex(0) {}

void Menu::onClockTick() {
    clockUpdateFlag = true;
}

void Menu::begin() {
    tft_init();
    tft_clear(MENU_BG_COLOR);
    menuState = MENU_MAIN;
    show();
    clockTicker.attach(callback(this, &Menu::onClockTick), 1s);
}

void Menu::drawClock() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", t);
    tft_text(CLOCK_X, CLOCK_Y, timeStr, CLOCK_TEXT_COLOR, MENU_BG_COLOR);
}

void Menu::displayResetAndPrint(const char* text) {
    tft_init();
    tft_clear(MENU_BG_COLOR);
    if (text) {
        tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE, text, MENU_TEXT_COLOR, MENU_BG_COLOR);
    }
    drawClock();
}

void Menu::show() {
    char D_option[24];
    strcpy(D_option, _sdManager.isMounted() ? "D: Unmount SD card" : "D: Mount SD card");

    tft_clear(MENU_BG_COLOR);
    tft_text(MENU_TITLE_X, MENU_TITLE_Y, (char*)">>>------ Mushroomer Menu ------<<<", MENU_TEXT_COLOR, MENU_BG_COLOR);

    const char* options[] = {
        "1: FSM State",
        "2: Temp & Humidity",
        "3: Set RTC Time",
        "4: Save State to Log",
        "5: View Event Log",
        "6: Set lower humidity threshold",
        "A: Set lower temperature threshold",
        "B: Set upper temperature threshold",
        "C: Force ventilation",
        D_option,
        "#: Return to Menu"
    };

    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + i * MENU_OPTION_SPACING, (char*)options[i], MENU_TEXT_COLOR, MENU_BG_COLOR);
    }
    drawClock();
}

void Menu::handleKey(char key) {
    const char* goBack = "Press # to go back to the menu";

    switch (menuState) {
        case MENU_MAIN:
            switch (key) {
                case '1': showFSMState(); break;
                case '2': showTempHumidity(); break;
                case '3': displayResetAndPrint(">>>---- Setting RTC via keypad ----<<<"); menuState = MENU_SET_RTC; break;
                case '4': saveEventLog(); break;
                case '5': viewEventLog(); break;
                case '6': handleSetLowerHumidityThreshold(); break;
                case 'A': handleSetLowerTemperatureThreshold(); break;
                case 'B': handleSetUpperTemperatureThreshold(); break;
                case 'C': forceVentilation(); break;
                case 'D': handleSDCard(); break;
                case '#': show(); break;
                default: {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "Invalid key: '%c'", key);
                    displayResetAndPrint(buffer);
                    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 3 * MENU_OPTION_SPACING, (char*)goBack, MENU_TEXT_COLOR, MENU_BG_COLOR);
                }
            }
            break;

        case MENU_SET_RTC:
            if (key >= '0' && key <= '9' && _rtcInputIndex < 14) {
                _rtcInputStr[_rtcInputIndex++] = key;
                _rtcInputStr[_rtcInputIndex] = '\0';
                displayResetAndPrint("Set RTC Time:");
                tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 1 * MENU_OPTION_SPACING, (char*)"YYYYMMDDHHMMSS", GRAY_TEXT_COLOR, MENU_BG_COLOR);
                tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, _rtcInputStr, INFO_TEXT_COLOR, MENU_BG_COLOR);
            }
            if (_rtcInputIndex == 14) {
                struct tm rtcTime;
                char temp[5];

                memcpy(temp, _rtcInputStr, 4); temp[4] = '\0'; rtcTime.tm_year = atoi(temp) - 1900;
                memcpy(temp, _rtcInputStr + 4, 2); temp[2] = '\0'; rtcTime.tm_mon = atoi(temp) - 1;
                memcpy(temp, _rtcInputStr + 6, 2); temp[2] = '\0'; rtcTime.tm_mday = atoi(temp);
                memcpy(temp, _rtcInputStr + 8, 2); temp[2] = '\0'; rtcTime.tm_hour = atoi(temp);
                memcpy(temp, _rtcInputStr + 10, 2); temp[2] = '\0'; rtcTime.tm_min = atoi(temp);
                memcpy(temp, _rtcInputStr + 12, 2); temp[2] = '\0'; rtcTime.tm_sec = atoi(temp);
                rtcTime.tm_isdst = -1;

                set_time(mktime(&rtcTime));
                displayResetAndPrint("RTC successfully set!");
                thread_sleep_for(1500);
                show();
                menuState = MENU_MAIN;
                _rtcInputIndex = 0;
            }
            break;
    }
}

void Menu::showFSMState() {
    displayResetAndPrint(">>>------ FSM State ------<<<");
    const char* stateStr = _fsm.getStateString();
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "<> %s", stateStr);
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
}

void Menu::showTempHumidity() {
    displayResetAndPrint(">>>------ Temp & Hum ------<<<");
    float temp = _fsm.getLastTemp();
    float hum = _fsm.getLastHumidity();
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Temp  CURRENT READING: %.2fC", temp);
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
    snprintf(buffer, sizeof(buffer), "      LOWER THRESHOLD: %.2fC", _fsm.getTempLowerThreshold());
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 3 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
    snprintf(buffer, sizeof(buffer), "      UPPER THRESHOLD: %.2fC", _fsm.getTempUpperThreshold());
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 4 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
    snprintf(buffer, sizeof(buffer), "Hum   CURRENT READING: %.2f%%", hum);
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 6 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
    snprintf(buffer, sizeof(buffer), "      LOWER THRESHOLD: %.2f%%", _fsm.getHumLowerThreshold());
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 7 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
}

void Menu::showCurrentTime() {
    displayResetAndPrint(">>>---- Current Time ----<<<");
    time_t now = time(NULL);
    struct tm* localTime = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, buffer, MENU_TEXT_COLOR, MENU_BG_COLOR);
}

void Menu::saveEventLog() {
    displayResetAndPrint(">>>------ Event Log ------<<<");
    _logger.addEntry(_fsm.getStateString(), _fsm.getLastTemp(), _fsm.getLastHumidity());
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, (char*)"Entry saved to log", MENU_TEXT_COLOR, MENU_BG_COLOR);
}

void Menu::viewEventLog() {
    displayResetAndPrint(">>>------ Event Log ------<<<");
    int total = _logger.getEntryCount();
    if (total == 0) {
        tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, (char*)"No entries yet", MENU_TEXT_COLOR, MENU_BG_COLOR);
        return;
    }

    int toShow = (total >= LOG_ENTRIES_TO_DISPLAY) ? LOG_ENTRIES_TO_DISPLAY : total;
    int start = total - toShow;

    for (int i = 0; i < toShow; i++) {
        const char* line = _logger.getDisplayEntry(i, toShow);
        int y = MENU_OPTION_Y_BASE + i * MENU_OPTION_SPACING;
        tft_text(MENU_OPTION_X, y, line, MENU_TEXT_COLOR, MENU_BG_COLOR);
    }
}

void Menu::handleSetUpperTemperatureThreshold() {
    displayResetAndPrint("Set max temp (e.g. 25):");
    char str[3] = {}; int index = 0;
    while (index < 2) {
        char k = _keypad.update();
        if (k != '\0') {
            str[index++] = k; str[index] = '\0';
            char ch[2] = {k, '\0'};
            tft_text(MENU_OPTION_X + (index - 1) * 10, MENU_OPTION_Y_BASE + 3 * MENU_OPTION_SPACING, ch, INFO_TEXT_COLOR, MENU_BG_COLOR);
        }
    }
    _fsm.setTempUpperThreshold(atoi(str));
    if (_fsm.getTempUpperThreshold() <= _fsm.getTempLowerThreshold())
        _fsm.setTempLowerThreshold(_fsm.getTempUpperThreshold() - 10);
    char msg[32];
    snprintf(msg, sizeof(msg), "Max temp set: %f C", _fsm.getTempUpperThreshold());
    displayResetAndPrint(msg);
}

void Menu::handleSetLowerTemperatureThreshold() {
    displayResetAndPrint("Set min temp (e.g. 15):");
    char str[3] = {}; int index = 0;
    while (index < 2) {
        char k = _keypad.update();
        if (k != '\0') {
            str[index++] = k; str[index] = '\0';
            char ch[2] = {k, '\0'};
            tft_text(MENU_OPTION_X + (index - 1) * 10, MENU_OPTION_Y_BASE + 3 * MENU_OPTION_SPACING, ch, INFO_TEXT_COLOR, MENU_BG_COLOR);
        }
    }
    _fsm.setTempLowerThreshold(atof(str));
    if (_fsm.getTempLowerThreshold() >= _fsm.getTempUpperThreshold())
        _fsm.setTempUpperThreshold(_fsm.getTempLowerThreshold() + 10);
    char msg[32];
    snprintf(msg, sizeof(msg), "Min temp set: %f C", _fsm.getTempLowerThreshold());
    displayResetAndPrint(msg);
}

void Menu::handleSetLowerHumidityThreshold() {
    displayResetAndPrint("Set min hum (e.g. 40):");
    char str[3] = {}; int index = 0;
    while (index < 2) {
        char k = _keypad.update();
        if (k != '\0') {
            str[index++] = k; str[index] = '\0';
            char ch[2] = {k, '\0'};
            tft_text(MENU_OPTION_X + (index - 1) * 10, MENU_OPTION_Y_BASE + 3 * MENU_OPTION_SPACING, ch, INFO_TEXT_COLOR, MENU_BG_COLOR);
        }
    }
    _fsm.setHumLowerThreshold(atof(str));
    char msg[32];
    snprintf(msg, sizeof(msg), "Min hum set: %f%%", _fsm.getHumLowerThreshold());
    displayResetAndPrint(msg);
}

void Menu::forceVentilation() {
    displayResetAndPrint(">>> Force ventilation requested <<<");
    _fsm.forceVentilation();
    tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING, (char*)"Fan will turn on soon", MENU_TEXT_COLOR, MENU_BG_COLOR);
}

void Menu::handleSDCard() {
    displayResetAndPrint(">>> SD Card Action <<<");
    if (_sdManager.isMounted()) {
        tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING,
                 _sdManager.safeRemove() ? (char*)"You may now remove SD" : (char*)"Error removing SD",
                 _sdManager.safeRemove() ? MENU_TEXT_COLOR : ERROR_TEXT_COLOR, MENU_BG_COLOR);
    } else {
        tft_text(MENU_OPTION_X, MENU_OPTION_Y_BASE + 2 * MENU_OPTION_SPACING,
                 _sdManager.begin() ? (char*)"SD mounted OK" : (char*)"Mount failed",
                 _sdManager.begin() ? CLOCK_TEXT_COLOR : ERROR_TEXT_COLOR, MENU_BG_COLOR);
    }
}
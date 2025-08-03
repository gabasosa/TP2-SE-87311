#ifndef _MENU_H_
#define _MENU_H_

#include "FSMController.h"
#include "EventLogger.h"
#include "KeypadScanner.h"
#include "SDCardManager.h"

class Menu {
public:
    Menu(FSMController& fsm, EventLogger& logger, KeypadScanner& keypad, SDCardManager& sdManager);
    void begin();
    void handleKey(char key);
    void drawClock();

private:
    static void onClockTickStatic();  // static callback
    void onClockTick();               // instance handler
    static Menu* instance;            // static pointer to current instance

    void show();
    void displayResetAndPrint(const char* text);

    void showFSMState();
    void showTempHumidity();
    void showCurrentTime();
    void saveEventLog();
    void viewEventLog();

    void handleSetUpperTemperatureThreshold();
    void handleSetLowerTemperatureThreshold();
    void handleSetLowerHumidityThreshold();

    void forceVentilation();
    void handleSDCard();

    enum MenuState {
        MENU_MAIN,
        MENU_SET_RTC
    } menuState;

    FSMController& _fsm;
    EventLogger& _logger;
    KeypadScanner& _keypad;
    SDCardManager& _sdManager;
    char _rtcInputStr[15];
    int _rtcInputIndex;

};

#endif
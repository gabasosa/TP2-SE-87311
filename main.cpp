#include "mbed.h"
#include "arm_book_lib.h"

#include "sensors/dht22.h"
#include "events/event_logger.h"
#include "fsm/fsm.h"
#include "keypad/keypad.h"
#include "menu/menu.h"
#include "sdcard/sdcard.h"

// Global
extern volatile bool clockUpdateFlag;

// Tickers
Ticker fsmTicker;
Ticker logTicker;

// Flags
volatile bool fsmFlag = false;
volatile bool logFlag = false;

void onFsmTick() {
    fsmFlag = true;
}

void onLogTick() {
    logFlag = true;
}

int main() {
    // Initialize peripherals
    matrixKeypadInit();
    DHT22_Init();
    FSM_Init();
    EventLogger_Init();

    if (SDCard_Init() == false) {
        printf("[ERROR] Could not initialize SD card!\n");
    } else {
        printf("[INFO] SD card initialized successfully.\n");
    }

    Menu_Init();

    fsmTicker.attach(&onFsmTick, 2s);
    logTicker.attach(&onLogTick, 300s);

    while (true) {
        if (fsmFlag) {
            fsmFlag = false;
            FSM_Update();
        }

        if (logFlag) {
            logFlag = false;
            EventLogger_AddEntry(FSM_GetState(), FSM_GetLastTemp(), FSM_GetLastHumidity());
        }

        if (clockUpdateFlag) {
            clockUpdateFlag = false;
            Menu_DrawClock();
        }

        char key = matrixKeypadUpdate();
        if (key != '\0') {
            Menu_HandleKey(key);
        }

        thread_sleep_for(10);
    }
}
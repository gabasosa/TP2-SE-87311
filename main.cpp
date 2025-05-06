#include "mbed.h"
#include "arm_book_lib.h"

#include "keypad/keypad.h"
#include "menu/menu.h"
#include "fsm/fsm.h"
#include "sensors/dht22.h"
#include "event_logger.h"

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
    // Initialize all modules
    matrixKeypadInit();
    Menu_Init();          // Initializes and displays the menu on the TFT
    DHT22_Init();
    FSM_Init();
    EventLogger_Init();

    fsmTicker.attach(&onFsmTick, 2s);
    logTicker.attach(&onLogTick, 5s);

    while (true) {
        // FSM state update
        if (fsmFlag) {
            fsmFlag = false;
            FSM_Update();
        }

        // Handle keypad input
        char key = matrixKeypadUpdate();
        if (key != '\0') {
            Menu_HandleKey(key);  // Refactored: no UART needed
        }

        thread_sleep_for(10);  // For debounce, aligned with TIME_INCREMENT_MS
    }
}
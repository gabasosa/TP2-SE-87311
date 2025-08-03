#include "mbed.h"
#include "arm_book_lib.h"

#include "config/pin_map.h"
#include "FSMController.h"
#include "ble/BLEManager.h"
#include "sensors/DHT22Sensor.h"
#include "modules/actuators/PeltierController.h"
#include "events/EventLogger.h"
#include "menu/Menu.h"
#include "keypad/KeypadScanner.h"
#include "sdcard/SDCardManager.h"
#include "lights/Lights.h"

// Global objects
DHT22Sensor dht(TEMP_HUM_SENSOR_PIN);
PeltierController peltier(PELTIER_RPWM_PIN, PELTIER_LPWM_PIN,
                          PELTIER_REN_PIN, PELTIER_LEN_PIN,
                          PELTIER_FAN1_CTRL_PIN, PELTIER_FAN2_CTRL_PIN);
Fan fan(FAN_RELAY_PIN, FAN_LED_INDICATOR);
Humidifier humidifier(HUMIDIFIER_CONTROL_PIN, HUMIDIFIER_LED_INDICATOR_PIN);
Flap flap(SERVO_FLAP_PIN);
FSMController fsm(dht, peltier, fan, humidifier, flap);
KeypadScanner keypad;
SDCardManager sdcard;
EventLogger logger(sdcard);
Menu menu(fsm, logger, keypad, sdcard);
Lights lights(LIGHT_LED_PIN);
BLEManager ble(BLE_UART_RX_PIN, BLE_UART_TX_PIN, fsm, lights);

// RTOS Threads
Thread fsmThread;
Thread bleThread;
Thread keypadThread;
Thread loggerThread;

// FSM thread loop
void fsmLoop() {
    while (true) {
        fsm.update();
        ThisThread::sleep_for(2s);
    }
}

// BLE thread loop
void bleLoop() {
    while (true) {
        ble.process();
        ThisThread::sleep_for(10ms);
    }
}

// Keypad thread loop
void keypadLoop() {
    while (true) {
        keypad.update();
        char key = keypad.scan();
        if (key != '\0') {
            menu.handleKey(key);
        }
        ThisThread::sleep_for(20ms);
    }
}

// Logger thread loop (automatic logging every 60s)
void loggerLoop() {
    while (true) {
        logger.addEntry(fsm.getStateString(), fsm.getLastTemp(), fsm.getLastHumidity());
        ThisThread::sleep_for(60s);
    }
}

int main() {
    printf("[BOOT] Mushroomer system started.\\n");

    // Initialize modules
    dht.begin();
    peltier.begin();
    fan.begin();
    humidifier.begin();
    flap.begin();
    fsm.begin();
    keypad.begin();
    logger.begin();
    sdcard.begin();
    ble.begin();
    menu.begin();

    // Start RTOS threads
    fsmThread.start(fsmLoop);
    bleThread.start(bleLoop);
    keypadThread.start(keypadLoop);
    loggerThread.start(loggerLoop);

    // Main thread can sleep or monitor status
    while (true) {
        ThisThread::sleep_for(1s);
    }
}
#ifndef _BLE_MANAGER_H_
#define _BLE_MANAGER_H_

#include "mbed.h"
#include "fsm/FSMController.h"
#include "lights/Lights.h"

class BLEManager {
public:
    BLEManager(PinName tx, PinName rx, FSMController& fsmRef, Lights& lightsRef, int baud = 9600);

    void begin();
    void process();

private:
    BufferedSerial _serial;
    FSMController& _fsm;
    Lights& _lights;

    void executeCommand(uint8_t cmd, float threshold);
};

#endif
#include "BLEManager.h"
#include "FSMController.h"
#include "Lights.h"
#include <cstdio>
#include <cstring>

BLEManager::BLEManager(PinName tx, PinName rx, FSMController& fsmRef, Lights& lightsRef, int baud)
    : _serial(tx, rx, baud), _fsm(fsmRef), _lights(lightsRef) {}

void BLEManager::begin() {
    printf("[BLE] Ready.\n");
}

void BLEManager::process() {
    char buf[64] = {0};
    ssize_t len = _serial.read(buf, sizeof(buf) - 1);

    if (len <= 0) {
        return;
    }

    buf[len] = '\0';

    if (buf[0] != '#') {
        return;
    }

    // Use for Debugging
    printf("[BLE] Full string: \"%s\"\n", buf);

    uint8_t cmd = 0;
    float threshold = 0.0f;
    int matched = sscanf(buf, "# %hhu %f", &cmd, &threshold);

    switch (matched) {
        case 1:
            executeCommand(cmd, 0.0f);
            break;
        case 2:
            executeCommand(cmd, threshold);
            break;
        default:
            // Use for Debugging
            //printf("[BLE] Invalid format\n");
            break;
    }
}

void BLEManager::executeCommand(uint8_t cmd, float threshold) {
    printf("[BLE] Executing command %d with threshold %.2f\n", cmd, threshold);

    switch (cmd) {
        case 1: {
            const char* stateStr = _fsm.getStateString();
            _serial.write("STATE ", 7);
            _serial.write(stateStr, strlen(stateStr));
            _serial.write("\r\n", 2);
            break;
        }

        case 2: {
            float temp = _fsm.getLastTemp();
            float hum = _fsm.getLastHumidity();
            char buf[64];
            snprintf(buf, sizeof(buf), "READ %.1f %.1f\r\n", temp, hum);
            _serial.write(buf, strlen(buf));
            break;
        }

        case 3: {
            char buf[64];
            snprintf(buf, sizeof(buf), "RTH %.1f %.1f %.1f\r\n",
                     _fsm.getHumLowerThreshold(),
                     _fsm.getTempLowerThreshold(),
                     _fsm.getTempUpperThreshold());
            _serial.write(buf, strlen(buf));
            break;
        }

        case 4: {
            _fsm.forceVentilation();
            _serial.write("CMD_FORCE_VENT_OK\r\n", 20);
            break;
        }

        case 5: {
            _lights.toggle();
            _serial.write("CMD_SWITCH_LIGHTS_OK\r\n", 22);
            break;
        }

        case 6: {
            _fsm.setHumLowerThreshold(threshold);
            printf("[BLE] Set HUM LOWER THRESHOLD = %.2f\n", threshold);
            _serial.write("CMD_SET_HUM_OK\r\n", 18);
            break;
        }

        case 7: {
            _fsm.setTempLowerThreshold(threshold);
            printf("[BLE] Set TEMP LOWER THRESHOLD: %.2f\n", threshold);
            _serial.write("CMD_LOWER_TEMP_OK\r\n", 21);
            break;
        }

        case 8: {
            _fsm.setTempUpperThreshold(threshold);
            printf("[BLE] Set TEMP UPPER THRESHOLD: %.2f\n", threshold);
            _serial.write("CMD_UPPER_TEMP_OK\r\n", 21);
            break;
        }

        default:
            printf("[BLE] Unknown command: %d\n", cmd);
            break;
    }
}
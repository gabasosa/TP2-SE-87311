#include "SDCardManager.h"

SDCardManager::SDCardManager()
    : _sd(SD_MOSI_PIN, SD_MISO_PIN, SD_SCK_PIN, SD_CS_PIN), _fs("sd"), _mounted(false) {}

/*
bool SDCardManager::begin() {
    if (_sd.init() != 0) return false;
    if (_fs.mount(&_sd) != 0) {
        _sd.deinit();
        return false;
    }

    _mounted = true;
    return true;
}
*/

bool SDCardManager::begin() {
    int initResult = _sd.init();
    if (initResult != 0) {
        printf("[SD] init() failed. Error: %d\n", initResult);
        return false;
    }

    int mountResult = _fs.mount(&_sd);
    if (mountResult != 0) {
        printf("[SD] mount() failed. Error: %d\n", mountResult);
        _sd.deinit();
        return false;
    }

    printf("[SD] Mount successful.\n");
    _mounted = true;
    return true;
}

bool SDCardManager::unmount() {
    if (!_mounted) return false;

    if (_fs.unmount() != 0) return false;
    _sd.deinit();
    _mounted = false;
    return true;
}

bool SDCardManager::safeRemove() {
    return unmount();
}

bool SDCardManager::isMounted() const {
    return _mounted;
}

SDBlockDevice& SDCardManager::getBlockDevice() {
    return _sd;
}

FATFileSystem& SDCardManager::getFileSystem() {
    return _fs;
}
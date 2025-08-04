#ifndef _SDCARD_MANAGER_H_
#define _SDCARD_MANAGER_H_

#include "SDBlockDevice.h"
#include "FATFileSystem.h"
#include "pin_map.h"

class SDCardManager {
public:
    SDCardManager();

    bool begin();          // Mount
    bool safeRemove();     // Unmount
    bool isMounted() const;

    SDBlockDevice& getBlockDevice();
    FATFileSystem& getFileSystem();

private:
    SDBlockDevice _sd;
    FATFileSystem _fs;
    bool _mounted;

    bool unmount();
};

#endif
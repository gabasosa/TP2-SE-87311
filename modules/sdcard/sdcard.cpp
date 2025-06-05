#include "sdcard.h"
#include "pin_map.h"

static bool sdMounted = false;

SDBlockDevice sd(SD_MOSI_PIN, SD_MISO_PIN, SD_SCK_PIN, SD_CS_PIN);
FATFileSystem fs("sd");

bool SDCard_Init() {
    if (sd.init() != 0) return false;
    if (fs.mount(&sd) != 0) {
        sd.deinit();
        return false;
    }

    sdMounted = true;
    return true;
}

bool SDCard_Unmount() {
    if (!sdMounted) return false;

    if (fs.unmount() != 0) return false;
    sd.deinit();
    sdMounted = false;
    return true;
}

bool SDCard_IsMounted() {
    return sdMounted;
}

bool SDCard_SafeRemove() {
    return SDCard_Unmount();
}
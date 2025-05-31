#ifndef _SDCARD_H_
#define _SDCARD_H_

#include "SDBlockDevice.h"
#include "FATFileSystem.h"

extern SDBlockDevice sd;
extern FATFileSystem fs;

bool SDCard_Init();
bool SDCard_SafeRemove();
bool SDCard_IsMounted();

#endif
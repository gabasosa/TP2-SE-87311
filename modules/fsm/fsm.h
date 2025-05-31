#ifndef _FSM_H_
#define _FSM_H_

#include "fsm/state_defs.h"

extern int tempThresholdUpper;
extern int humThresholdLower;

void FSM_Init();
void FSM_Update();

MushroomerState_t FSM_GetState();     // for logging/display
float FSM_GetLastTemp();              // expose for display
float FSM_GetLastHumidity();
const char* FSM_StateToString(MushroomerState_t state);

#endif
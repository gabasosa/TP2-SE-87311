#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "mbed.h"

//=====[Defines]===============================================================

#define KEYPAD_NUMBER_OF_ROWS 4
#define KEYPAD_NUMBER_OF_COLS 4
#define DEBOUNCE_KEY_TIME_MS  40
#define TIME_INCREMENT_MS     10

//=====[Public data types]=====================================================

typedef enum {
    MATRIX_KEYPAD_SCANNING,
    MATRIX_KEYPAD_DEBOUNCE,
    MATRIX_KEYPAD_KEY_HOLD_PRESSED
} matrixKeypadState_t;

//=====[Public function prototypes]============================================

void matrixKeypadInit();
char matrixKeypadScan();
char matrixKeypadUpdate();

#endif // _KEYPAD_H_

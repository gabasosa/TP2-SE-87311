#ifndef _KEYPAD_SCANNER_H_
#define _KEYPAD_SCANNER_H_

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================

#define KEYPAD_NUMBER_OF_ROWS 4
#define KEYPAD_NUMBER_OF_COLS 4
#define DEBOUNCE_KEY_TIME_MS  40
#define TIME_INCREMENT_MS     10

typedef enum {
    MATRIX_KEYPAD_SCANNING,
    MATRIX_KEYPAD_DEBOUNCE,
    MATRIX_KEYPAD_KEY_HOLD_PRESSED
} matrixKeypadState_t;

class KeypadScanner {
public:
    KeypadScanner();
    void begin();
    char scan();
    char update();

private:
    DigitalOut _rowPins[KEYPAD_NUMBER_OF_ROWS];
    DigitalIn  _colPins[KEYPAD_NUMBER_OF_COLS];

    matrixKeypadState_t _state;
    int _debounceTime;
    char _lastKeyPressed;

    const char _indexToCharArray[KEYPAD_NUMBER_OF_ROWS * KEYPAD_NUMBER_OF_COLS] = {
        '1', '2', '3', 'A',
        '4', '5', '6', 'B',
        '7', '8', '9', 'C',
        '*', '0', '#', 'D',
    };
};

#endif // _KEYPAD_SCANNER_H_

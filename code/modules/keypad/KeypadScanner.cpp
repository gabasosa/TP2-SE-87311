#include "KeypadScanner.h"
#include "config/pin_map.h"

KeypadScanner::KeypadScanner()
    : _state(MATRIX_KEYPAD_SCANNING), _debounceTime(0), _lastKeyPressed('\0'),
      _rowPins{KEYPAD_ROW_1_PIN, KEYPAD_ROW_2_PIN, KEYPAD_ROW_3_PIN, KEYPAD_ROW_4_PIN},
      _colPins{KEYPAD_COL_1_PIN, KEYPAD_COL_2_PIN, KEYPAD_COL_3_PIN, KEYPAD_COL_4_PIN}
{}

void KeypadScanner::begin()
{
    _state = MATRIX_KEYPAD_SCANNING;
    for (int i = 0; i < KEYPAD_NUMBER_OF_COLS; i++) {
        _colPins[i].mode(PullUp);
    }
}

char KeypadScanner::scan()
{
    for (int row = 0; row < KEYPAD_NUMBER_OF_ROWS; row++) {
        for (int i = 0; i < KEYPAD_NUMBER_OF_ROWS; i++) {
            _rowPins[i] = ON;
        }
        _rowPins[row] = OFF;

        for (int col = 0; col < KEYPAD_NUMBER_OF_COLS; col++) {
            if (_colPins[col] == OFF) {
                return _indexToCharArray[row * KEYPAD_NUMBER_OF_ROWS + col];
            }
        }
    }
    return '\0';
}

char KeypadScanner::update()
{
    char keyDetected = '\0';
    char keyReleased = '\0';

    switch (_state) {
        case MATRIX_KEYPAD_SCANNING:
            keyDetected = scan();
            if (keyDetected != '\0') {
                _lastKeyPressed = keyDetected;
                _debounceTime = 0;
                _state = MATRIX_KEYPAD_DEBOUNCE;
            }
            break;

        case MATRIX_KEYPAD_DEBOUNCE:
            if (_debounceTime >= DEBOUNCE_KEY_TIME_MS) {
                keyDetected = scan();
                if (keyDetected == _lastKeyPressed) {
                    _state = MATRIX_KEYPAD_KEY_HOLD_PRESSED;
                } else {
                    _state = MATRIX_KEYPAD_SCANNING;
                }
            }
            _debounceTime += TIME_INCREMENT_MS;
            break;

        case MATRIX_KEYPAD_KEY_HOLD_PRESSED:
            keyDetected = scan();
            if (keyDetected != _lastKeyPressed) {
                if (keyDetected == '\0') {
                    keyReleased = _lastKeyPressed;
                }
                _state = MATRIX_KEYPAD_SCANNING;
            }
            break;

        default:
            begin();
            break;
    }

    return keyReleased;
}
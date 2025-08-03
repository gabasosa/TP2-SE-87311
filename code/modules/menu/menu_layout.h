#ifndef MENU_LAYOUT_H
#define MENU_LAYOUT_H

#include <cstdint>

// Text colors
#define MENU_TEXT_COLOR     TFT_BLACK
#define MENU_BG_COLOR       TFT_WHITE
#define CLOCK_TEXT_COLOR    TFT_GREEN
#define ERROR_TEXT_COLOR    TFT_RED
#define INFO_TEXT_COLOR     TFT_ORANGE
#define GRAY_TEXT_COLOR     TFT_GRAY

// Clock position
#define CLOCK_X             254
#define CLOCK_Y             230

#define LOG_ENTRIES_TO_DISPLAY   5

// Main menu layout
#define MENU_TITLE_X        10
#define MENU_TITLE_Y        10

#define MENU_OPTION_X       10
#define MENU_OPTION_Y_BASE  30
#define MENU_OPTION_SPACING 20

#define MENU_RETURN_Y       230

// Max number of static menu options (update if you add more)
#define MENU_OPTION_COUNT   11

// Raw calibration values
#define RAW_TOUCH_X_MIN  0
#define RAW_TOUCH_X_MAX  320
#define RAW_TOUCH_Y_MIN  -10
#define RAW_TOUCH_Y_MAX  100

// Best set known
//#define RAW_TOUCH_X_MIN  -30
//#define RAW_TOUCH_X_MAX  240
//#define RAW_TOUCH_Y_MIN  -10
//#define RAW_TOUCH_Y_MAX  110

struct Button {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    char key;
};

// Button areas for touchscreen
static const Button menuButtons[MENU_OPTION_COUNT] = {
    {MENU_OPTION_X, 30, 240, 18, '1'},
    {MENU_OPTION_X, 50, 240, 18, '2'},
    {MENU_OPTION_X, 70, 240, 18, '3'},
    {MENU_OPTION_X, 90, 240, 18, '4'},
    {MENU_OPTION_X, 110, 240, 18, '5'},
    {MENU_OPTION_X, 130, 240, 18, '6'},
    {MENU_OPTION_X, 150, 240, 18, 'A'},
    {MENU_OPTION_X, 170, 240, 18, 'B'},
    {MENU_OPTION_X, 190, 240, 18, 'C'},
    {MENU_OPTION_X, 210, 240, 18, 'D'},
    {MENU_OPTION_X, MENU_RETURN_Y, 240, 18, '#'},
};

#endif // MENU_LAYOUT_H

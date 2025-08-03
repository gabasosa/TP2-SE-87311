#include "touch/TouchInput.h"
#include "menu/menu_layout.h"
#include "pin_map.h"

TouchInput::TouchInput(PinName clk, PinName cs, PinName din, PinName dout, PinName irq)
    : _touch(clk, cs, din, dout, irq) {}

void TouchInput::begin() {
    _touch.InitTouch(LANDSCAPE);
    _touch.SetPrecision(PREC_MEDIUM);
}

void TouchInput::normalize(int rawX, int rawY, int& x, int& y) {
    int tempX = rawX;
    int tempY = rawY;

    x = (tempX - RAW_TOUCH_X_MIN) * 320 / (RAW_TOUCH_X_MAX - RAW_TOUCH_X_MIN);
    y = (tempY - RAW_TOUCH_Y_MIN) * 240 / (RAW_TOUCH_Y_MAX - RAW_TOUCH_Y_MIN);

    if (x < 0) x = 0;
    if (x >= 320) x = 319;
    if (y < 0) y = 0;
    if (y >= 240) y = 239;
}

char TouchInput::update() {
    if (!_touch.DataAvailable()) return '\0';

    if (_touch.Read()) {
        int rawX = _touch.GetX();
        int rawY = _touch.GetY();
        int x, y;
        normalize(rawX, rawY, x, y);

        for (size_t i = 0; i < sizeof(menuButtons) / sizeof(menuButtons[0]); ++i) {
            const Button& b = menuButtons[i];
            if (x >= b.x && x <= (b.x + b.w) && y >= b.y && y <= (b.y + b.h)) {
                return b.key;
            }
        }
    }

    return '\0';
}
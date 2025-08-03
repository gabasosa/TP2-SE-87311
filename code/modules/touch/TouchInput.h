#ifndef _TOUCH_INPUT_H_
#define _TOUCH_INPUT_H_

#include "UTouch/UTouch.h"
#include "mbed.h"

class TouchInput {
public:
    TouchInput(PinName clk, PinName cs, PinName din, PinName dout, PinName irq);
    
    void begin();
    char update();  // Returns '\0' if no key pressed, or detected char key

private:
    void normalize(int rawX, int rawY, int& x, int& y);

    UTouch _touch;
};

#endif
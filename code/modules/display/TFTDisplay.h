#ifndef _TFTDISPLAY_H_
#define _TFTDISPLAY_H_

#include <stdint.h>

class TFTDisplay {
public:
    void begin();
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void clear(uint16_t color);
    void putChar(int x, int y, char chr, uint16_t color, uint16_t bgcolor);
    void printText(int x, int y, const char* text, uint16_t color, uint16_t bgcolor);
    void drawHLine(int x1, int y, int x2, uint16_t color);
    void drawVLine(int x, int y1, int y2, uint16_t color);
    void drawBox(int x1, int y1, int x2, int y2, uint16_t color);
    void fillBox(int x1, int y1, int x2, int y2, uint16_t color);
    void drawBitmap(int x, int y, const unsigned char* bitmap);
};

#endif
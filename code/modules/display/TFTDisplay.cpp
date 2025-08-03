#include "TFTDisplay.h"
#include "ILI9341/tft.h"

void TFTDisplay::begin() {
    tft_init();
}

void TFTDisplay::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    tft_set_window(x0, y0, x1, x1);
}

void TFTDisplay::clear(uint16_t color) {
    tft_clear(color);
}

void TFTDisplay::putChar(int x, int y, char chr, uint16_t color, uint16_t bgcolor) {
    tft_put_char(x, y, chr, color, bgcolor);
}

void TFTDisplay::printText(int x, int y, const char* text, uint16_t color, uint16_t bgcolor) {
    tft_text(x, y, text, color, bgcolor);
}

void TFTDisplay::drawHLine(int x1, int y, int x2, uint16_t color) {
    tft_hline(x1, y, x2, color);
}

void TFTDisplay::drawVLine(int x, int y1, int y2, uint16_t color) {
    tft_vline(x, y1, y2, color);
}

void TFTDisplay::drawBox(int x1, int y1, int x2, int y2, uint16_t color) {
    tft_box(x1, y1, x2, y2, color);
}

void TFTDisplay::fillBox(int x1, int y1, int x2, int y2, uint16_t color) {
    tft_boxfill(x1, y1, x2, y2, color);
}

void TFTDisplay::drawBitmap(int x, int y, const unsigned char* bitmap) {
    draw_bmp_4bpp(bitmap, x, y);
}
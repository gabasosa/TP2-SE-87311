/*===================================================================*/
/*                                                                   */
/*  tft.cpp : TFT(ILI9341) function                                  */
/*                                                                   */
/*  2016/1/20  Racoon                                                */
/*                                                                   */
/*===================================================================*/
#include "mbed.h"
#include "config/pin_map.h"
#include "tft.h"

DigitalOut  cs(DISPLAY_CS_PIN, PullUp);   // TFT chipselect pin
DigitalOut  dc(DISPLAY_DC_PIN, PullUp);   // TFT data command select pin
DigitalOut  rst(DISPLAY_RST_PIN, PullUp);  // TFT reset pin

/*-------------------------------------------------------------------*/
/*  Write command                                                    */

/*-------------------------------------------------------------------*/
void write_cmd(uint8_t cmd)
{
    dc = 0;
    spi_write(cmd);
}

/*-------------------------------------------------------------------*/
/*  Write data                                                       */

/*-------------------------------------------------------------------*/
void write_data(uint8_t data)
{
    dc = 1;
    spi_write(data);
}

/*-------------------------------------------------------------------*/
/*  TFT reset                                                        */

/*-------------------------------------------------------------------*/
void tft_reset()
{
    ThisThread::sleep_for(200ms);
    cs = 1;
    dc = 1;
    rst = 1;
    ThisThread::sleep_for(200ms);
    rst = 0;
    wait_us(10);
    rst = 1;
    ThisThread::sleep_for(120ms);
    cs = 0;
    ThisThread::sleep_for(10ms);

    write_cmd(0x3A);    // Pixel Format
    write_data(0x55);   // 16bit Color
    write_cmd(0xB1);    // Frame Control
    write_data(0);
    write_data(0x1f);

    write_cmd(0x36);    // Memory Access Control
    write_data(0xE8);   // MY MX MV BGR
    write_cmd(0x11);    // Sleep Out
    ThisThread::sleep_for(5ms);

    write_cmd(0x29);    // Display On
}

/*-------------------------------------------------------------------*/
/*  Set windows size, start memory write                             */

/*-------------------------------------------------------------------*/
void tft_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    write_cmd(0x2A);    // Column Address Set
    write_data(x0 >> 8);
    write_data(x0);
    write_data(x1 >> 8);
    write_data(x1);

    write_cmd(0x2B);    // Page Address Set
    write_data(y0 >> 8);
    write_data(y0);
    write_data(y1 >> 8);
    write_data(y1);

    write_cmd(0x2C);    // Memory Write
    wait_us(20);

    dc = 1;
}

/*-------------------------------------------------------------------*/
/*  Clear screen                                                     */

/*-------------------------------------------------------------------*/
void tft_clear(uint16_t color)
{
    uint16_t  pixel[320];
    
    tft_set_window(0, 0, TFT_WIDTH, TFT_HEIGHT);

    //for (int i = 0; i < TFT_WIDTH * TFT_HEIGHT; ++i) {
        //pixel[i++] = color;
    //}
    
    
    for (int i = 0; i < TFT_HEIGHT; i++) {
        for (int j = 0; j < TFT_WIDTH; j++) {
            pixel[j] = color;
        }
        HAL_SPI_Transmit(&SpiHandle, (uint8_t*)pixel, TFT_WIDTH * 2, 100);        
    }
    
}

/*-------------------------------------------------------------------*/
/*  Put char                                                         */

/*-------------------------------------------------------------------*/
void tft_put_char(int x, int y, char chr, uint16_t color, uint16_t bgcolor)
{
    if (chr < 0x20 || chr > 0x7f) {
        chr = 0x3f;
    }
    else {
        chr = (chr < 0x60) ? chr - 0x20 : chr - 0x40;
    }

    tft_set_window(x, y, x + 7, y + 6);

    for (int dy = 0; dy < 7; ++dy) {
        unsigned char   img = chrimg[chr][dy];
        for (int dx = 0; dx < 8; ++dx) {
            if (img & 0x80) {
                spi_writew(color);
            }
            else {
                spi_writew(bgcolor);
            }

            img <<= 1;
        }
    }
}

/*-------------------------------------------------------------------*/
/*  Text out                                                         */

/*-------------------------------------------------------------------*/
void tft_text(int x, int y, const char* text, uint16_t color, uint16_t bgcolor)
{
    while (*text != 0) {
        tft_put_char(x, y, *text, color, bgcolor);
        x += 8;
        text++;
    }
}

/*-------------------------------------------------------------------*/
/*  Horizontal Line                                                        */

/*-------------------------------------------------------------------*/
void tft_hline(int x1, int y, int x2, uint16_t color)
{
    uint16_t  pixel[320];
    int       i = 0;
    
    tft_set_window(x1, y, x2, y);

    for (; x1 < x2; ++x1) {
        //spi_writew(color);
        pixel[i++] = color;
    }
    HAL_SPI_Transmit(&SpiHandle, (uint8_t*)pixel, (i - 1) * 2, 100);        
}

/*-------------------------------------------------------------------*/
/*  Vertical Line                                                        */

/*-------------------------------------------------------------------*/
void tft_vline(int x, int y1, int y2, uint16_t color)
{
    uint16_t  pixel[240];
    int       i = 0;
    
    tft_set_window(x, y1, x, y2);

    for (; y1 < y2; ++y1) {
        //spi_writew(color);
        pixel[i++] = color;
    }
    HAL_SPI_Transmit(&SpiHandle, (uint8_t*)pixel, (i - 1) * 2, 100);        
}

/*-------------------------------------------------------------------*/
/*  Box                                                              */

/*-------------------------------------------------------------------*/
void tft_box(int x1, int y1, int x2, int y2, uint16_t color)
{
    tft_hline(x1, y1, x2, color);
    tft_vline(x1, y1, y2, color);
    tft_vline(x2, y1, y2, color);
    tft_hline(x1, y2, x2, color);
}

/*-------------------------------------------------------------------*/
/*  Box Fill                                                        */

/*-------------------------------------------------------------------*/
void tft_boxfill(int x1, int y1, int x2, int y2, uint16_t color)
{
    uint16_t  pixel[320];
    int i;
    int j;
    
    tft_set_window(x1, y1, x2, y2);

    //for (int i = 0; i < (x2 - x1 + 1) * (y2 - y1 + 1); ++i) {
        //spi_writew(color);
    //}
    
    for (i = 0; i < (y2 - y1 + 1); i++) {
        for (j = 0; j < (x2 - x1 + 1); j++) {
            pixel[j] = color;
        }
        HAL_SPI_Transmit(&SpiHandle, (uint8_t*)pixel, (j - 1) * 2, 100);        
    }
}

/*-------------------------------------------------------------------*/
/*  Draw 4bit BMP                                                    */

/*-------------------------------------------------------------------*/
bool draw_bmp_4bpp(const unsigned char* imgdata, int x, int y)
{
    BITMAPFILEHEADER*   bf = (BITMAPFILEHEADER*)imgdata;
    BITMAPINFOHEADER*   bi = (BITMAPINFOHEADER *) (imgdata + sizeof(BITMAPFILEHEADER));

    if (bi->biBitCount != 4) {
        return false;
    }

    unsigned char*  pRGBPal = (unsigned char*)imgdata + sizeof(BITMAPFILEHEADER) + bi->biSize;
    unsigned short  palette[16];

    for (int i = 0; pRGBPal < imgdata + bf->bfOffBits && i < 16; ++i) {
        unsigned short  r, g, b;
        b = *pRGBPal++ >> 3;
        g = *pRGBPal++ >> 2;
        r = *pRGBPal++ >> 3;
        pRGBPal++;
        palette[i] = ((g & 7) << 13) | (b << 8) | (r << 3) | (g >> 3);
    }

    unsigned short  HLine[320];
    int             linesize = (bi->biWidth / 2 + 3) & 0xfffc;

    tft_set_window(x, y, x + bi->biWidth - 1, y + bi->biHeight - 1);

    unsigned char*  bmp;

    for (int y = bi->biHeight - 1; y >= 0; --y) {
        bmp = (unsigned char*)imgdata + bf->bfOffBits + y * linesize;

        for (int x = 0; x < bi->biWidth; ++x) {
            char    pal;
            if (x & 1) {
                pal = *bmp & 0xf;
                bmp++;
            }
            else {
                pal = *bmp >> 4;
            }

            HLine[x] = palette[pal];
        }

        HAL_SPI_Transmit(&SpiHandle, (uint8_t*)HLine, bi->biWidth * 2, 100);
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*  Initialize TFT                                                   */

/*-------------------------------------------------------------------*/
void tft_init(void)
{
    spi_init();

    tft_reset();
}
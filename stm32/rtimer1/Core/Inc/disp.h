/**
 * Module disp
 * driving display GC9A01
 * 
 * this display driver is heavily based on:
 *   https://github.com/offpic/GC9A01-DMA-TFT-STM32-STM32F401-CLOCK
 *   https://github.com/GolinskiyKonstantin/STM32_Lib_TFT_ST7789
 */

#ifndef _DISP_H_
#define _DISP_H_

#include "main.h"
#include "fonts.h"

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

extern uint16_t disp_width, disp_height;
extern uint16_t disp_x_start, disp_y_start;

#define PI 3.141592

#define DISP_WIDTH 240
#define DISP_HEIGHT 240
#define DISP_XSTART 0							// смещение начала отсчета области
#define DISP_YSTART 0							// смещение начала отсчета области
#define DISP_DEF_ROTATION (DISP_MADCTL_BGR) // DISP_rotation( 0, 0, 0 )

#define DISP_RGB565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// - defined colors
#define DISP_BLACK 0x0000
#define DISP_BLUE 0x001F
#define DISP_RED 0xF800
#define DISP_GREEN 0x07E0
#define DISP_CYAN 0x07FF
#define DISP_MAGENTA 0xF81F
#define DISP_YELLOW 0xFFE0
#define DISP_WHITE 0xFFFF

// - Display Controller Commands
#define DISP_SLPIN 0x10
#define DISP_SLPOUT 0x11
#define DISP_INVOFF 0x20
#define DISP_INVON 0x21
#define DISP_DISPOFF 0x28
#define DISP_DISPON 0x29
#define DISP_CASET 0x2A
#define DISP_RASET 0x2B
#define DISP_RAMWR 0x2C
#define DISP_TEON 0x35   // Tearing effect line ON
#define DISP_MADCTL 0x36 // Memory data access control
#define DISP_COLMOD 0x3A // Pixel format set
#define DISP_RAMWR_CONT 0x3C

#define DISP_DisplayFunctionControl 0xB6
#define DISP_PWCTR1 0xC1 // Power control 1
#define DISP_PWCTR2 0xC3 // Power control 2
#define DISP_PWCTR3 0xC4 // Power control 3
#define DISP_PWCTR4 0xC9 // Power control 4
#define DISP_PWCTR7 0xA7 // Power control 7

#define DISP_FRAMERATE 0xE8
#define DISP_InnerReg1Enable 0xFE
#define DISP_InnerReg2Enable 0xEF

#define DISP_GAMMA1 0xF0 // Set gamma 1
#define DISP_GAMMA2 0xF1 // Set gamma 2
#define DISP_GAMMA3 0xF2 // Set gamma 3
#define DISP_GAMMA4 0xF3 // Set gamma 4

// - Values ​​passed as argument to DISP_COLMOD command
#define ColorMode_RGB_16bit 0x50
#define ColorMode_RGB_18bit 0x60
#define ColorMode_MCU_12bit 0x03
#define ColorMode_MCU_16bit 0x05
#define ColorMode_MCU_18bit 0x06

#define DISP_MADCTL_MY 0x80
#define DISP_MADCTL_MX 0x40
#define DISP_MADCTL_MV 0x20
#define DISP_MADCTL_ML 0x10
#define DISP_MADCTL_BGR 0x08
#define DISP_MADCTL_MH 0x04

// - public functions
void disp_init(void);
void disp_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
void disp_enter_sleep_mode(void);
void disp_exit_sleep_mode(void);
void disp_inversion_mode(uint8_t mode);
void disp_clear(void);
void disp_fill_rectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void disp_fill_screen(uint16_t color);
void disp_set_backlight(uint8_t value);
void disp_power(uint8_t on);

void disp_draw_rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void disp_draw_filled_rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t fill_color);
void disp_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void disp_draw_thick_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, uint8_t thick);
void disp_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void disp_draw_filled_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
void disp_draw_pixel(int16_t x, int16_t y, uint16_t color);
void disp_draw_filled_Circle(int16_t x0, int16_t y0, int16_t radius, uint16_t fill_color);
void disp_draw_circle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
void disp_draw_char(uint16_t x, uint16_t y, uint16_t text_color, uint16_t bg_color, uint8_t transparent_Bg, FontDef_t *font, uint8_t multiplier, unsigned char ch);
void disp_print(uint16_t x, uint16_t y, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font, uint8_t multiplier, char *str);
void disp_rotation(uint8_t rotation, uint8_t vert_mirror, uint8_t horiz_mirror);
void disp_draw_bitmap(int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h, uint16_t color);
void disp_draw_circle_helper(int16_t x0, int16_t y0, int16_t radius, int8_t quadrant_mask, uint16_t color);
void disp_draw_filled_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);
void disp_draw_filled_round_rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t corner_radius, uint16_t color);
void disp_draw_round_rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t corner_radius, uint16_t color);
void disp_draw_arc(int16_t x0, int16_t y0, int16_t radius, int16_t start_angle, int16_t end_angle, uint16_t color, uint8_t thick);

#endif // _DISP_H_

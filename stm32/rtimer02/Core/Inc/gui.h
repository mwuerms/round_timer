/**
 * module gui "GUI elements on display"
 * Martin, 2025-03-11
 */

#ifndef _DISP_GUI_H_
#define _DISP_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "disp.h"

// - public functions ----------------------------------------------------------

/**
 * draw a little cross to show the position
 * @param	x0		center x position
 * @param	y0		center y position
 * @param	color	of the cross
 */
void gui_draw_position_cross(uint16_t x0, uint16_t y0, uint16_t color);

/**
 * print some text on the display
 * @param	text	to print
 * @param	x0		x position, left upper conrner
 * @param	y0		y position, left upper conrner
 * @param	center_x	=true: do center text, =false: use x0
 * @param	space	space between box and text
 * @param	text_color
 * @param	bg_color
 * @param	transparent_bg	=false: do not use bg_colr
 */
void gui_text(char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);

/**
 * draw a text box on the display
 * @param	text	to print
 * @param	x0		x position, left upper conrner
 * @param	y0		y position, left upper conrner
 * @param	center	=true: do center text, =false: use x0
 * @param	space	space between box and text, also corner radius
 * @param	text_color 	color of the text and the box
 * @param	bg_color
 * @param	transparent_bg	=false: do not use bg_color
 */
void gui_text_box(char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t space, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);

/**
 * draw a progress bar
 * @param	progress	0 ... 100
 * @param	x0		x position, left upper conrner
 * @param	y0		y position, left upper conrner
 * @param	size_x  width
 * @param	size_y  heigth
 * @param	center_x	=true: do center text, =false: use x0
 * @param	color
 */
void gui_progress_bar(uint16_t progress, uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint8_t center_x, uint16_t color);

/**
 * draw a symbol
 * @param   symbol  the symbol to draw, see GUI_SYMBOL_STOP_SIZE_16x16 ...
 * @param	s   poiner to sprite
 * @param	x0  x position, left upper conrner
 * @param	y0  y position, left upper conrner
 * @param	center_x	=true: do center symbol, =false: use x0
 * @param	color   see dISP_COLOR_...
 * @param	bg_color   see dISP_COLOR_...
 * @param	transparent_bg	=false: do not use bg_color
 */
#define GUI_SYMBOL_STOP_SIZE_16x16 (0)
#define GUI_SYMBOL_STOP_SIZE_32x32 (1)
#define GUI_SYMBOL_PLAY_SIZE_16x16 (2)
#define GUI_SYMBOL_PLAY_SIZE_32x32 (3)
#define GUI_SYMBOL_PAUSE_SIZE_16x16 (4)
#define GUI_SYMBOL_PAUSE_SIZE_32x32 (5)
#define GUI_SYMBOL_ALARM_SIZE_16x16 (6)
#define GUI_SYMBOL_ALARM_SIZE_32x32 (7)
void gui_draw_symbol(uint16_t symbol, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t color, uint16_t bg_color, uint8_t transparent_bg);

/**
 * draw a cursor using rounded box
 * @param	x0  x position, left upper conrner
 * @param	y0  y position, left upper conrner
 * @param	size_x  width
 * @param	size_y  height
 * @param	space	space between box and text, also corner radius
 * @param	color   see dISP_COLOR_...
 */
void gui_box(uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint16_t space, uint16_t color);
#define gui_cursor(x0, y0, size_x, size_y, space, color) gui_box(x0, y0, size_x, size_y, space, color)

#ifdef __cplusplus
}
#endif

#endif // _DISP_GUI_H_

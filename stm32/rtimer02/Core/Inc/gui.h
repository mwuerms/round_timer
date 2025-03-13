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

#define GUI_ELEMENT_TEXT_SIZE (16)
typedef struct {
	uint16_t type;
	uint16_t flags;
	uint16_t x0, y0; // origin (x,y) position, left upper corner
	uint16_t size_x, size_y; // size of the gui element
	uint16_t border; // space around the gui element
	uint16_t x1, y1, x2, y2; // total positions (x1,y1) = (x0-border,y0-border), (x2,y2) = (x0+size_x+border,y0+size_y+border)
	uint16_t fg_color, bg_color; // colors of fore ground and back ground
	struct {
		// element specific content, see type, static for now
		struct {
			FontDef_t *font;
			char str[GUI_ELEMENT_TEXT_SIZE];
			uint16_t flags;
		} text;
		uint16_t symbol;
	} content;
} gui_element_t;

#define GUI_ELEMENT_TYPE_BOX (10) // a box with sharp corners
#define GUI_ELEMENT_TYPE_ROUND_BOX (11) // a box with round corners
#define GUI_ELEMENT_TYPE_TEXT_LABEL (100) // plain text, no border
#define GUI_ELEMENT_TYPE_TEXT_BOX (101) // text with border (round box)
#define GUI_ELEMENT_TYPE_PROGRESS_BAR (200) // a progress bar
#define GUI_ELEMENT_TYPE_SYMBOL (200) // only a symbol, may have a border (round box)
#define GUI_ELEMENT_TYPE_TEXT_SYMBOL (201) // only a symbol, may have a border (round box)

#define BITn(n) (1 << n)

#define GUI_ELEMENT_FLAG_REDRAW	BITn(0)
#define GUI_ELEMENT_FLAG_TRANSPARENT_BG BITn(1) // ignore bg_color
#define GUI_ELEMENT_FLAG_SHOW_BORDER  BITn(2)// uses as cursor

#define GUI_ELEMENT_TEXT_FLAG_CENTER BITn(0)

// - public functions ----------------------------------------------------------

void gui_element_set_redraw(gui_element_t *g);
void gui_draw_element(gui_element_t *g);
#define gui_clear_area(x0, y0, size_x, size_y, color) disp_fill_rectangle(x0, y0, size_x, size_y, color)
#define gui_clear() disp_clear()

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
 * @param	x0		x position, left upper corner
 * @param	y0		y position, left upper corner
 * @param	center_x	=true: do center text, =false: use x0
 * @param	space	space between box and text
 * @param	text_color
 * @param	bg_color
 * @param	transparent_bg	=false: do not use bg_color
 */
void gui_text(char *text, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);
void gui_text_with_center_x(char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);
void gui_element_init_text_label(gui_element_t *g, char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);

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

#define GUI_SYMBOL_STOP (0)
#define GUI_SYMBOL_PLAY (1)
#define GUI_SYMBOL_PAUSE (2)
#define GUI_SYMBOL_ALARM (3)

void gui_draw_symbol(uint16_t symbol, uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint8_t center_x, uint16_t color, uint16_t bg_color, uint8_t transparent_bg);
void gui_element_init_small_text_symbol(gui_element_t *g, char *text, uint16_t symbol, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);
void gui_element_init_large_text_symbol(gui_element_t *g, char *text, uint16_t symbol, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font);

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

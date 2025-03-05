/**
 * timer_app, timer application
 * Martin, 2025-03-05
 */

#include "timer_app.h"
#include <string.h>
#include "disp.h"

#define NB_TIMERS (4)
static struct {
	uint16_t state;
} timer_app_ctrl;

typedef struct {
	uint16_t secs, ms; // max secs: 65535 = 18:12:15 (HH:MM:SS), ms: 0 ... 999 Milliseconds
	uint8_t hour, min, sec, x; // x: dummy byte
} time_x_t;

static time_x_t timers[NB_TIMERS];

void timer_app_init(void) {
	memset(timers, 0, sizeof(timers));

}

#define TIMER_APP_STR_FONT Font_16x26
#define TIMER_APP_NUM_FONT Font_16x28

#include <string.h>
/**
 * print some text on the display
 * @param	text	to print
 * @param	x0		x position, left upper conrner
 * @param	y0		y position, left upper conrner
 * @param	center	=true: do center text, =false: use x0
 * @param	space	space between box and text
 * @param	text_color
 * @param	bg_color
 * @param	transparent_bg	=false: do not use bg_colr
 */
void gui_text(char *text, uint16_t x0, uint16_t y0, uint8_t center, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	uint16_t fwid, nb_chars;
	// some checks first
	if((x0 > DISP_WIDTH) || (y0 > DISP_HEIGHT)){
		// error, outside display
		return;
	}
	if(text == NULL) {
		// error, no text
		return;
	}
	fwid = (uint16_t)font->FontWidth;
	//fhi  = (uint16_t)font->FontHeight;
	nb_chars = (uint16_t)strlen(text)-1;
	if(center) {
		// center text
		x0 = DISP_CENTER_X - (fwid * nb_chars)/2 - fwid/2;
	}
	disp_print(x0, y0, text_color, bg_color, transparent_bg, font, 0, text);
}

/**
 * draw a text box on the display
 * @param	text	to print
 * @param	x0		x position, left upper conrner
 * @param	y0		y position, left upper conrner
 * @param	center	=true: do center text, =false: use x0
 * @param	space	space between box and text
 * @param	text_color 	color of the text and the box
 * @param	bg_color
 * @param	transparent_bg	=false: do not use bg_colr
 */
void gui_text_box(char *text, uint16_t x0, uint16_t y0, uint8_t center, uint16_t space, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	uint16_t fwid, fhi, nb_chars;
	// some checks first
	if((x0 > DISP_WIDTH) || (y0 > DISP_HEIGHT)){
		// error, outside display
		return;
	}
	if(text == NULL) {
		// error, no text
		return;
	}
	fwid = (uint16_t)font->FontWidth;
	fhi  = (uint16_t)font->FontHeight;
	nb_chars = (uint16_t)strlen(text)-1;
	if(center) {
		// center text
		x0 = DISP_CENTER_X - (fwid * nb_chars)/2 - fwid/2;
	}
	disp_draw_round_rectangle(x0-space, y0-space, (fwid * (nb_chars+1))+2*space, fhi+2*space, space, text_color);
	disp_print(x0, y0, text_color, bg_color, transparent_bg, font, 0, text);
}

/**
 * draw a progress bar
 * @param	progress	0 ... 100
 * @param	x0		x position, left upper conrner
 * @param	y0		y position, left upper conrner
 * @param	width
 * @param	heigth
 * @param	center	=true: do center text, =false: use x0
 * @param	color
 */
void gui_progress_bar(uint16_t progress, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint8_t center, uint16_t color) {
	uint16_t x1, space = 4;
	if(center) {
		x0 = DISP_CENTER_X - width/2;
	}
	if(progress > 100) {
		progress = 100;
	}
	disp_draw_round_rectangle(x0-space, y0-space, width+2*space, height+2*space, 2*space, color);
	x1 = x0 + width;
	width = (width * progress)/100;
	x0 = x1 - width; // right aligned
	disp_draw_filled_round_rectangle(x0, y0, width, height, space, color);
}

void timer_app_draw_bg(void) {
	/*
	int16_t x0, y0, fwid, fhi, str_len;
	fwid = TIMER_APP_STR_FONT.FontWidth;
	fhi  = TIMER_APP_STR_FONT.FontHeight;
	str_len = 5; // "Timer"
	x0 = DISP_CENTER_X-(fwid*str_len+16)/2;
	y0 = 30;

	disp_draw_round_rectangle(x0, y0, fwid*str_len+16, fhi+8, 4, DISP_COLOR_WHITE);
	*/
	disp_draw_line(DISP_CENTER_X, 0, DISP_CENTER_X, 240, DISP_COLOR_YELLOW);

	disp_draw_circle(DISP_CENTER_X, DISP_CENTER_Y, 118, DISP_COLOR_WHITE);
	gui_text_box("Timer", 0, 30, 1, 5, DISP_COLOR_WHITE, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_text("00:00", 0,  80, 1, DISP_COLOR_WHITE, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_text("stop", 0, 160, 1, DISP_COLOR_WHITE, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);

	gui_progress_bar(73, 40, 130, 120, 20, 1, DISP_COLOR_WHITE);
}

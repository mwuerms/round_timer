/**
 * timer_app, timer application
 * Martin, 2025-03-05
 */

#include "timer_app.h"
#include <string.h>
#include "gui.h"
#include "disp.h"
#include <string.h>

#define TIMER_APP_STR_FONT Font_16x26
#define TIMER_APP_NUM_FONT Font_16x28

#define TIMER_APP_NB_TIMERS (4)
static struct {
	uint16_t state;
    uint8_t current_timer;
    uint8_t cursor_pos;
} timer_app_ctrl;
#define TIMER_APP_STATE_STOP (0)
#define TIMER_APP_STATE_RUNNING (1)
#define TIMER_APP_STATE_PAUSE (2)
#define TIMER_APP_STATE_ALARM (3)

typedef struct {
	uint16_t secs, ms; // max secs: 65535 = 18:12:15 (HH:MM:SS), ms: 0 ... 999 Milliseconds
	uint8_t hour, min, sec, x; // x: dummy byte
} time_x_t;

static time_x_t timers[TIMER_APP_NB_TIMERS];

void timer_app_init(void) {
	memset(timers, 0, sizeof(timers));
}

#define TIMER_APP_GUI_COLOR DISP_COLOR_CYAN

void timer_app_draw(void) {
	//disp_draw_line(DISP_CENTER_X, 0, DISP_CENTER_X, 240, DISP_COLOR_YELLOW); // center line for debug

	//gui_text_box("Timer", 0, 30, 1, 5, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_text("Timer", 0, 30, 1, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_text("1", DISP_CENTER_X-3*16-16/2, 60, 0, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_16x16, DISP_CENTER_X-3*16-16/2, 90, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_cursor(DISP_CENTER_X-3*16-16/2, 60, 16, 1*30+16, 4, TIMER_APP_GUI_COLOR);

	gui_text("2", DISP_CENTER_X-1*16-16/2, 60, 0, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_draw_symbol(GUI_SYMBOL_PLAY_SIZE_16x16, DISP_CENTER_X-1*16-16/2, 90, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_cursor(DISP_CENTER_X-1*16-16/2, 60, 16, 1*30+16, 4, TIMER_APP_GUI_COLOR);

	gui_text("3", DISP_CENTER_X+1*16-16/2, 60, 0, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_draw_symbol(GUI_SYMBOL_PAUSE_SIZE_16x16, DISP_CENTER_X+1*16-16/2, 90, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_cursor(DISP_CENTER_X+1*16-16/2, 60, 16, 1*30+16, 4, TIMER_APP_GUI_COLOR);

	gui_text("4", DISP_CENTER_X+3*16-16/2, 60, 0, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_draw_symbol(GUI_SYMBOL_ALARM_SIZE_16x16, DISP_CENTER_X+3*16-16/2, 90, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_cursor(DISP_CENTER_X+3*16-16/2, 60, 16, 1*30+16, 4, TIMER_APP_GUI_COLOR);

	// box around
	gui_box(DISP_CENTER_X-3*16-16/2-4, 30, 7*16+8, 2*30+16+4, 4, TIMER_APP_GUI_COLOR);

	disp_draw_circle(DISP_CENTER_X, DISP_CENTER_Y, 118, TIMER_APP_GUI_COLOR);

	gui_text("00:00", 0,  120, 1, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);
	gui_cursor(DISP_CENTER_X-2*16-16/2, 120, 2*16, 26, 4, DISP_COLOR_RED);
	gui_cursor(DISP_CENTER_X+1*16-16/2, 120, 2*16, 26, 4, DISP_COLOR_GREEN);

	gui_progress_bar(36, 20, 156, 180, 20, 1, TIMER_APP_GUI_COLOR);
	gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_32x32, 0, 190, 1, TIMER_APP_GUI_COLOR, 0, 0);
	gui_cursor(DISP_CENTER_X-32/2, 190, 32, 32, 4, DISP_COLOR_YELLOW);
}

#if 0
	// unused at the moment
	/*
	//gui_text("stop", 0, 160, 1, TIMER_APP_GUI_COLOR, DISP_COLOR_BLACK, 0, &TIMER_APP_STR_FONT);

	//gui_draw_symbol(symbol, x0, y0, center_x, color, bg_color, transparent_bg)
	gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_16x16, 1*64, 160, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_32x32, 1*64+20, 160, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_draw_symbol(GUI_SYMBOL_PLAY_SIZE_16x16, 2*64, 160, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_draw_symbol(GUI_SYMBOL_PLAY_SIZE_32x32, 2*64+21, 160, 0, TIMER_APP_GUI_COLOR, 0, 0);

	gui_draw_symbol(GUI_SYMBOL_PAUSE_SIZE_16x16, 1*64, 200, 0, TIMER_APP_GUI_COLOR, 0, 0);
	gui_draw_symbol(GUI_SYMBOL_PAUSE_SIZE_32x32, 1*64+20, 200, 0, TIMER_APP_GUI_COLOR, 0, 0);
	//gui_draw_symbol(GUI_SYMBOL_ALARM_SIZE_16x16, 2*64, 200, 0, TIMER_APP_GUI_COLOR, 0, 0);
	//gui_draw_symbol(GUI_SYMBOL_ALARM_SIZE_32x32, 1*64+21, 195, 0, TIMER_APP_GUI_COLOR, 0, 0);


	gui_draw_position_cross(40, 130, DISP_COLOR_RED);

	gui_draw_position_cross(1*64, 160, DISP_COLOR_RED);
	gui_draw_position_cross(1*64+20, 160, DISP_COLOR_RED);
	gui_draw_position_cross(2*64, 160, DISP_COLOR_RED);
	gui_draw_position_cross(2*64+20, 160, DISP_COLOR_RED);

	gui_draw_position_cross(1*64, 200, DISP_COLOR_RED);
	gui_draw_position_cross(1*64+16, 200+16, DISP_COLOR_RED);
	gui_draw_position_cross(1*64+20, 200, DISP_COLOR_RED);
	gui_draw_position_cross(1*64+20+32, 200+32, DISP_COLOR_RED);

	gui_draw_position_cross(2*64, 200, DISP_COLOR_RED);
	gui_draw_position_cross(2*64+20, 200, DISP_COLOR_RED);
	*/

}
#endif

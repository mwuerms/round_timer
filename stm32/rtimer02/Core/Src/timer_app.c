/**
 * timer_app, timer application
 * Martin, 2025-03-05
 */

#include "timer_app.h"
#include <string.h>
#include "gui.h"
#include "disp.h"
#include "FreeRTOS.h"
#include "str_buf.h"

#define TIMER_APP_GUI_FG_COLOR DISP_COLOR_CYAN
#define TIMER_APP_GUI_BG_COLOR DISP_COLOR_BLACK

#define TIMER_APP_NB_TIMERS (4)
#define TIMER_APP_NB_GUI_ELEMENTS (8)
static struct {
    uint8_t current_timer;
    uint8_t cursor_pos;
    struct {
    	uint8_t flags; // .0= 0: nothing to do, =1: redraw
    } title;
    struct {
    	uint8_t flags; // .0 =0: nothing to do, =1: redraw, .1 = 1: current cursor pos
    } small_symbols[TIMER_APP_NB_TIMERS];
    struct {
    	uint8_t m, s;
    	char text[16];
    	uint8_t flags; // .0= 0: nothing to do, =1: redraw
    } time;
    struct {
    	uint8_t procent;
    	uint8_t flags; // .0= 0: nothing to do, =1: redraw
    } progress;
    struct {
    	uint8_t flags; // .0= 0: nothing to do, =1: redraw
    } large_symbol;
    gui_element_t g[TIMER_APP_NB_GUI_ELEMENTS];
} timer_app_gui_ctrl;
#define TIMER_APP_STATE_STOP (0)
#define TIMER_APP_STATE_RUNNING (1)
#define TIMER_APP_STATE_PAUSE (2)
#define TIMER_APP_STATE_ALARM (3)

#define TIMER_APP_CURSOR_POS0 (0)

#define TIMER_APP_GUI_TITLE (0)
#define TIMER_APP_GUI_SMALL_SYMBOL0 (1)
#define TIMER_APP_GUI_SMALL_SYMBOL1 (2)
#define TIMER_APP_GUI_SMALL_SYMBOL2 (3)
#define TIMER_APP_GUI_SMALL_SYMBOL3 (4)
#define TIMER_APP_GUI_TIME (5)
#define TIMER_APP_GUI_PROG_BAR (6)
#define TIMER_APP_GUI_LARGE_SYMBOL (7)

typedef struct {
	uint16_t state;
	uint16_t secs, ms; // max secs: 65535 = 18:12:15 (HH:MM:SS), ms: 0 ... 999 Milliseconds
	uint8_t hour, min, sec, x; // x: dummy byte
} time_x_t;

static time_x_t timers[TIMER_APP_NB_TIMERS];
#define TIMER_APP_STR_SIZE (16)
static char timer_app_str[TIMER_APP_STR_SIZE];

void timer_app_tim_func(void *argument);
osTimerId_t timer_app_tim_handle;
const osTimerAttr_t timer_app_tim_attributes = {
  .name = "timer_app_timer"
};

void timer_app_init(void) {
	timer_app_gui_ctrl.current_timer = 0;
	timer_app_gui_ctrl.cursor_pos = 0;
	timer_app_gui_ctrl.title.flags = 0x01;
	timer_app_gui_ctrl.small_symbols[0].flags = 0x01;
	timer_app_gui_ctrl.small_symbols[1].flags = 0x01;
	timer_app_gui_ctrl.small_symbols[2].flags = 0x01;
	timer_app_gui_ctrl.small_symbols[3].flags = 0x01;
	timer_app_gui_ctrl.time.flags = 0x01;
	timer_app_gui_ctrl.progress.flags = 0x01;
	timer_app_gui_ctrl.large_symbol.flags = 0x01;

	timer_app_gui_ctrl.small_symbols[timer_app_gui_ctrl.current_timer].flags |= 0x02;

	timers[0].state = TIMER_APP_STATE_STOP;
	timers[0].min = 10;
	timers[0].sec = 19;
	timers[1].state = TIMER_APP_STATE_RUNNING;
	timers[1].min = 22;
	timers[1].sec = 02;
	timers[2].state = TIMER_APP_STATE_PAUSE;
	timers[2].min = 33;
	timers[2].sec = 43;
	timers[3].state = TIMER_APP_STATE_ALARM;
	timers[3].min = 94;
	timers[3].sec = 14;

	// title
	gui_element_init_text_label(&timer_app_gui_ctrl.g[TIMER_APP_GUI_TITLE], "Timer", 0, 18, 1, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_16x26);
	gui_element_set_redraw(&timer_app_gui_ctrl.g[TIMER_APP_GUI_TITLE]);
	gui_element_init_small_text_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL0], "1", GUI_SYMBOL_STOP, DISP_CENTER_X-6*16+8, 50, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
	gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL0]);
	gui_element_init_small_text_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL1], "2", GUI_SYMBOL_STOP, DISP_CENTER_X-3*16+8, 50, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
	gui_element_set_redraw(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL1]);
	gui_element_init_small_text_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL2], "3", GUI_SYMBOL_PAUSE, DISP_CENTER_X+0*16+8, 50, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
	gui_element_set_redraw(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL2]);
	gui_element_init_small_text_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL3], "4", GUI_SYMBOL_PAUSE, DISP_CENTER_X+3*16+8, 50, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
	gui_element_set_redraw(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL3]);


	gui_element_init_progress_bar(&timer_app_gui_ctrl.g[TIMER_APP_GUI_PROG_BAR], DISP_CENTER_X-180/2, 156, 180, 20, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0);
	gui_element_set_redraw(&timer_app_gui_ctrl.g[TIMER_APP_GUI_PROG_BAR]);

	gui_element_init_large_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL], GUI_SYMBOL_STOP, DISP_CENTER_X-32/2, 190, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0);
	gui_element_set_redraw(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL]);

	timer_app_tim_handle = osTimerNew(timer_app_tim_func, osTimerPeriodic, NULL, &timer_app_tim_attributes);
	osTimerStart(timer_app_tim_handle, 1000/portTICK_PERIOD_MS);
}

static inline void timer_app_draw_state_small_symbol(uint16_t state, uint16_t x0, uint16_t y0, uint16_t color) {
	return;
	/*
	switch(state) {
	case TIMER_APP_STATE_STOP:
		gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_16x16, x0, y0, 0, color, 0, 0);
		break;
	case TIMER_APP_STATE_RUNNING:
		gui_draw_symbol(GUI_SYMBOL_PLAY_SIZE_16x16, x0, y0, 0, color, 0, 0);
		break;
	case TIMER_APP_STATE_PAUSE:
		gui_draw_symbol(GUI_SYMBOL_PAUSE_SIZE_16x16, x0, y0, 0, color, 0, 0);
		break;
	case TIMER_APP_STATE_ALARM:
		gui_draw_symbol(GUI_SYMBOL_ALARM_SIZE_16x16, x0, y0, 0, color, 0, 0);
		break;
	}
	*/
}

static inline void timer_app_draw_state_large_symbol(uint16_t state, uint16_t x0, uint16_t y0, uint16_t color) {
	return;
	/*
	switch(state) {
	case TIMER_APP_STATE_STOP:
		gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_32x32, x0, y0, 0, color, 0, 0);
		break;
	case TIMER_APP_STATE_RUNNING:
		gui_draw_symbol(GUI_SYMBOL_PLAY_SIZE_32x32, x0, y0, 0, color, 0, 0);
		break;
	case TIMER_APP_STATE_PAUSE:
		gui_draw_symbol(GUI_SYMBOL_PAUSE_SIZE_32x32, x0, y0, 0, color, 0, 0);
		break;
	case TIMER_APP_STATE_ALARM:
		gui_draw_symbol(GUI_SYMBOL_ALARM_SIZE_32x32, x0, y0, 0, color, 0, 0);
		break;
	}
	*/
}

void timer_app_draw_all(void) {
	return;
}

void timer_app_draw_old(void) {
	uint8_t m, s;
	//disp_draw_line(DISP_CENTER_X, 0, DISP_CENTER_X, 240, DISP_COLOR_YELLOW); // center line for debug

	if(timer_app_gui_ctrl.title.flags & 0x01) {
		timer_app_gui_ctrl.title.flags &= ~0x01;
		disp_clear();
		disp_draw_circle(DISP_CENTER_X, DISP_CENTER_Y, 118, TIMER_APP_GUI_FG_COLOR);
		gui_text_with_center_x("Timer", 0, 20, 1, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_16x26);
	}

	if(timer_app_gui_ctrl.small_symbols[0].flags & 0x01) {
		timer_app_gui_ctrl.small_symbols[0].flags &= ~0x01;
		gui_clear_area(DISP_CENTER_X-6*16+8-4, 50-4, 32+2*4, 16+2*4, TIMER_APP_GUI_BG_COLOR);

		gui_text_with_center_x("1", DISP_CENTER_X-6*16+8, 50, 0, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
		timer_app_draw_state_small_symbol(timers[0].state, DISP_CENTER_X-5*16+8, 50, TIMER_APP_GUI_FG_COLOR);
		if(timer_app_gui_ctrl.small_symbols[0].flags & 0x02) {
			gui_cursor(DISP_CENTER_X-6*16+8, 50, 32, 16, 4, TIMER_APP_GUI_FG_COLOR);
		}
	}

	if(timer_app_gui_ctrl.small_symbols[1].flags & 0x01) {
		timer_app_gui_ctrl.small_symbols[1].flags &= ~0x01;
		gui_clear_area(DISP_CENTER_X-3*16+8-4, 50-4, 32+2*4, 16+2*4, TIMER_APP_GUI_BG_COLOR);

		gui_text_with_center_x("2", DISP_CENTER_X-3*16+8, 50, 0, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
		timer_app_draw_state_small_symbol(timers[1].state, DISP_CENTER_X-2*16+8, 50, TIMER_APP_GUI_FG_COLOR);
		if(timer_app_gui_ctrl.small_symbols[1].flags & 0x02) {
			gui_cursor(DISP_CENTER_X-3*16+8, 50, 32, 16, 4, TIMER_APP_GUI_FG_COLOR);
		}
	}

	if(timer_app_gui_ctrl.small_symbols[2].flags & 0x01) {
		timer_app_gui_ctrl.small_symbols[2].flags &= ~0x01;
		gui_clear_area(DISP_CENTER_X+0*16+8-4, 50-4, 32+2*4, 16+2*4, TIMER_APP_GUI_BG_COLOR);

		gui_text_with_center_x("3", DISP_CENTER_X+0*16+8, 50, 0, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
		timer_app_draw_state_small_symbol(timers[2].state, DISP_CENTER_X+1*16+8, 50, TIMER_APP_GUI_FG_COLOR);
		if(timer_app_gui_ctrl.small_symbols[2].flags & 0x02) {
			gui_cursor(DISP_CENTER_X+0*16+8, 50, 32, 16, 4, TIMER_APP_GUI_FG_COLOR);
		}
	}

	if(timer_app_gui_ctrl.small_symbols[3].flags & 0x01) {
		timer_app_gui_ctrl.small_symbols[3].flags &= ~0x01;
		gui_clear_area(DISP_CENTER_X+3*16+8-4, 50-4, 32+2*4, 16+2*4, TIMER_APP_GUI_BG_COLOR);

		gui_text_with_center_x("4", DISP_CENTER_X+3*16+8, 50, 0, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_11x18);
		timer_app_draw_state_small_symbol(timers[3].state, DISP_CENTER_X+4*16+8, 50, TIMER_APP_GUI_FG_COLOR);
		if(timer_app_gui_ctrl.small_symbols[3].flags & 0x02) {
			gui_cursor(DISP_CENTER_X+3*16+8, 50, 32, 16, 4, TIMER_APP_GUI_FG_COLOR);
		}
	}

	if(timer_app_gui_ctrl.time.flags & 0x01) {
		timer_app_gui_ctrl.time.flags &= ~0x01;
		gui_clear_area(DISP_CENTER_X-2*16-16/2-4, 120-4, 5*16+2*4, 26+2*4, TIMER_APP_GUI_BG_COLOR);

		m = timers[timer_app_gui_ctrl.current_timer].min;
		s = timers[timer_app_gui_ctrl.current_timer].sec;
		str_buf_clear(timer_app_str, TIMER_APP_STR_SIZE);
		str_buf_append_uint8(timer_app_str, TIMER_APP_STR_SIZE, m);
		str_buf_append_char(timer_app_str, TIMER_APP_STR_SIZE, ':');
		str_buf_append_uint8(timer_app_str, TIMER_APP_STR_SIZE, s);

		//sprintf(timer_app_str, "%02d:%02d", m, s);
		//gui_text("00:00", 0,  120, 1, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_16x26);
		gui_text_with_center_x(timer_app_str, 0,  120, 1, TIMER_APP_GUI_FG_COLOR, TIMER_APP_GUI_BG_COLOR, 0, &Font_16x26);
		gui_cursor(DISP_CENTER_X-2*16-16/2, 120, 2*16, 26, 4, DISP_COLOR_RED);
		gui_cursor(DISP_CENTER_X+1*16-16/2, 120, 2*16, 26, 4, DISP_COLOR_GREEN);
	}

	if(timer_app_gui_ctrl.progress.flags & 0x01) {
		timer_app_gui_ctrl.progress.flags &= ~0x01;
		gui_clear_area(120-90-2, 156-2, 180+4, 24, TIMER_APP_GUI_BG_COLOR);

		gui_progress_bar(timers[timer_app_gui_ctrl.current_timer].min, 20, 156, 180, 20, 1, TIMER_APP_GUI_FG_COLOR);
	}

	if(timer_app_gui_ctrl.large_symbol.flags & 0x01) {
		timer_app_gui_ctrl.large_symbol.flags &= ~0x01;
		gui_clear_area(DISP_CENTER_X-32/2-4, 190-4, 40, 40, TIMER_APP_GUI_BG_COLOR);

		//gui_draw_symbol(GUI_SYMBOL_STOP_SIZE_32x32, 0, 190, 1, TIMER_APP_GUI_FG_COLOR, 0, 0);
		timer_app_draw_state_large_symbol(timers[timer_app_gui_ctrl.current_timer].state, DISP_CENTER_X-16, 190, TIMER_APP_GUI_FG_COLOR);
		gui_cursor(DISP_CENTER_X-32/2, 190, 32, 32, 4, DISP_COLOR_YELLOW);

		gui_draw_position_cross(DISP_CENTER_X-16, 190, DISP_COLOR_RED);
		gui_draw_position_cross(DISP_CENTER_X-16+32, 190+32, DISP_COLOR_RED);
	}
}

void timer_app_draw(void) {
	disp_draw_line(DISP_CENTER_X, 0, DISP_CENTER_X, 240, DISP_COLOR_YELLOW); // center line for debug
	uint16_t n;
	for(n = 0; n < TIMER_APP_NB_GUI_ELEMENTS; n++) {
		gui_draw_element(&timer_app_gui_ctrl.g[n]);
	}
}

void timer_app_tim_func(void *argument) {
	timer_app_gui_ctrl.small_symbols[timer_app_gui_ctrl.current_timer].flags &= ~0x02;
	timer_app_gui_ctrl.small_symbols[timer_app_gui_ctrl.current_timer].flags |=  0x01;
	timer_app_gui_ctrl.current_timer++;
	if(timer_app_gui_ctrl.current_timer >= TIMER_APP_NB_TIMERS) {
		timer_app_gui_ctrl.current_timer = 0;
	}
	timer_app_gui_ctrl.small_symbols[timer_app_gui_ctrl.current_timer].flags |= 0x01|0x02;
	// timer_app_gui_ctrl.current_timer changed, so change these values as well
	timer_app_gui_ctrl.time.flags = 0x01;
	timer_app_gui_ctrl.progress.flags = 0x01;
	timer_app_gui_ctrl.large_symbol.flags = 0x01;

	switch(timer_app_gui_ctrl.current_timer) {
	case 0:
		gui_element_show_no_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL3]);
		gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL0]);
		gui_element_set_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL], GUI_SYMBOL_STOP);
		gui_element_show_no_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL]);
		gui_element_set_progress(&timer_app_gui_ctrl.g[TIMER_APP_GUI_PROG_BAR], 100);
		break;
	case 1:
		gui_element_show_no_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL0]);
		gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL1]);
		gui_element_set_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL], GUI_SYMBOL_PAUSE);
		gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL]);
		gui_element_set_progress(&timer_app_gui_ctrl.g[TIMER_APP_GUI_PROG_BAR], 1);
		break;
	case 2:
		gui_element_show_no_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL1]);
		gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL2]);
		gui_element_set_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL], GUI_SYMBOL_ALARM);
		gui_element_show_no_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL]);
		gui_element_set_progress(&timer_app_gui_ctrl.g[TIMER_APP_GUI_PROG_BAR], 25);
		break;
	case 3:
		gui_element_show_no_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL2]);
		gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_SMALL_SYMBOL3]);
		gui_element_set_symbol(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL], GUI_SYMBOL_PLAY);
		gui_element_show_border(&timer_app_gui_ctrl.g[TIMER_APP_GUI_LARGE_SYMBOL]);
		gui_element_set_progress(&timer_app_gui_ctrl.g[TIMER_APP_GUI_PROG_BAR], 25);
		break;
	}
	timer_app_draw();
}

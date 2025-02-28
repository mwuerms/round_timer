/**
 * app, main application
 * Martin, 2025-02-26
 */

#include "app.h"
#include "scheduler.h"
#include "usr_in.h"
#include "disp.h"
#include "str_buf.h"

int8_t app_event_manager(uint8_t event, void *data);
uint8_t app_ev_task_id = 0;

#define APP_STR_LINE_SIZE (32)
static char app_str_line0[APP_STR_LINE_SIZE];

static task_t app_ev_task = {
	.name = "app_ev_task",
	.task = app_event_manager,
};

void app_init(void) {
	scheduler_add_task(&app_ev_task);
	app_ev_task_id = app_ev_task.tid;
	scheduler_start_task(app_ev_task_id);
}

static uint16_t cnt = 0;
int8_t app_event_manager(uint8_t event, void *data) {
	switch(event) {
	case EV_BTN0_PRESSED:
		disp_print(20, 80, DISP_RED, DISP_WHITE, 1, &Font_16x26, 0, "app BTN0 press");
		break;
	case EV_BTN0_RELEASED:
		disp_print(20, 80, DISP_GREEN, DISP_BLACK, 1, &Font_16x26, 0, "app BTN0 rel");
		break;
	case EV_BTN1_PRESSED:
		disp_print(20, 80, DISP_BLUE, DISP_WHITE, 1, &Font_16x26, 0, "app BTN1 press");
		break;
	case EV_BTN1_RELEASED:
		disp_print(20, 80, DISP_YELLOW, DISP_BLACK, 1, &Font_16x26, 0, "app BTN1 rel");
		break;
	case EV_BTN2_PRESSED:
		disp_print(20, 80, DISP_CYAN, DISP_WHITE, 1, &Font_16x26, 0, "app BTN2 press");
		break;
	case EV_BTN2_RELEASED:
		disp_print(20, 80, DISP_MAGENTA, DISP_BLACK, 1, &Font_16x26, 0, "app BTN2 rel");
		break;
	case EV_ENC_LEFT:
		str_buf_clear(app_str_line0, APP_STR_LINE_SIZE);
		str_buf_append_string(app_str_line0, APP_STR_LINE_SIZE, "LEFT:  ");
		cnt = usr_in_get_enc_abs_count();
		str_buf_append_int16(app_str_line0, APP_STR_LINE_SIZE, cnt);
		disp_print(20, 80, DISP_BLUE, DISP_GREEN, 1, &Font_16x26, 0, app_str_line0);
		break;
	case EV_ENC_RIGHT:
		str_buf_clear(app_str_line0, APP_STR_LINE_SIZE);
		str_buf_append_string(app_str_line0, APP_STR_LINE_SIZE, "RIGHT: ");
		cnt = usr_in_get_enc_abs_count();
		str_buf_append_int16(app_str_line0, APP_STR_LINE_SIZE, cnt);
		disp_print(20, 80, DISP_MAGENTA, DISP_BLACK, 1, &Font_16x26, 0, app_str_line0);
		break;

	case EV_START:
		break;
	case EV_STOP:
		break;
	//default: // unknown event, skip
	}
/*
	if(event == 100) {
		disp_draw_filled_circle(120, 120, 45, DISP_RED);
	}
	if(event == 101) {
		disp_draw_filled_circle(120, 120, 35, DISP_WHITE);
	}
	if(event == 102) {
		disp_draw_filled_circle(120, 120, 25, DISP_GREEN);
	}
	if(event == 103) {
		disp_draw_filled_circle(120, 120, 15, DISP_CYAN);
	}
	*/
	return TASK_RETURN_KEEP_RUNNING;
}

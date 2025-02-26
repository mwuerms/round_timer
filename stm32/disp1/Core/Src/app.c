/**
 * app, main application
 * Martin, 2025-02-26
 */

#include "app.h"
#include "scheduler.h"
#include "usr_in.h"
#include "disp.h"

int8_t app_event_manager(uint8_t event, void *data);
uint8_t app_ev_task_id = 0;

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
	if(event == EV_BTN0_PRESSED) {
		disp_print(20, 80, DISP_CYAN, DISP_BLACK, 0, &Font_16x26, 0, "app BTN0 press");
		cnt++;
		if(cnt == 8) {
			cnt = 0;
			scheduler_send_event(app_ev_task_id, 100, NULL);
		}
	}
	if(event == EV_BTN0_RELEASED) {
		disp_print(20, 80, DISP_GREEN, DISP_BLACK, 0, &Font_16x26, 0, "app BTN0 rel");
	}
	if(event == 100) {
		disp_draw_filled_circle(120, 120, 30, DISP_RED);
	}
	return TASK_RETURN_KEEP_RUNNING;
}

/**
 * app, main application
 * Martin, 2025-02-26
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app.h"
#include "usr_in.h"
#include "disp.h"
#include "str_buf.h"
#include "timer_app.h"

#define APP_STR_LINE_SIZE (32)
static char app_str_line0[APP_STR_LINE_SIZE];

int8_t app_event_task(void *argument);

osMessageQueueId_t app_ev_queue_handle;
const osMessageQueueAttr_t app_ev_queue_attributes = {
  .name = "app_ev_queue"
};
osThreadId_t app_ev_task_handle;
const osThreadAttr_t app_ev_task_attributes = {
  .name = "app_ev_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

void app_init(void) {
	app_ev_queue_handle = osMessageQueueNew (16, sizeof(uint16_t), &app_ev_queue_attributes);
	app_ev_task_handle = osThreadNew(app_event_task, NULL, &app_ev_task_attributes);

	timer_app_init();
	timer_app_draw();
}

int8_t app_event_task(void *argument) {
	uint16_t event;
	uint16_t cnt = 0;

	while(1) {
		if(osMessageQueueGet(app_ev_queue_handle, &event, NULL, osWaitForever) == osOK) {
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);

			switch(event) {
			case EV_BTN0_PRESSED:
				disp_print(20, 80, DISP_COLOR_RED, DISP_COLOR_WHITE, 1, &Font_16x26, 0, "app BTN0 press");
				break;
			case EV_BTN0_RELEASED:
				disp_print(20, 80, DISP_COLOR_GREEN, DISP_COLOR_BLACK, 1, &Font_16x26, 0, "app BTN0 rel");
				break;
			case EV_BTN1_PRESSED:
				disp_print(20, 80, DISP_COLOR_BLUE, DISP_COLOR_WHITE, 1, &Font_16x26, 0, "app BTN1 press");
				break;
			case EV_BTN1_RELEASED:
				disp_print(20, 80, DISP_COLOR_YELLOW, DISP_COLOR_BLACK, 1, &Font_16x26, 0, "app BTN1 rel");
				break;
			case EV_BTN2_PRESSED:
				disp_print(20, 80, DISP_COLOR_CYAN, DISP_COLOR_WHITE, 1, &Font_16x26, 0, "app BTN2 press");
				break;
			case EV_BTN2_RELEASED:
				disp_print(20, 80, DISP_COLOR_MAGENTA, DISP_COLOR_BLACK, 1, &Font_16x26, 0, "app BTN2 rel");
				break;
			case EV_ENC_LEFT:
				str_buf_clear(app_str_line0, APP_STR_LINE_SIZE);
				str_buf_append_string(app_str_line0, APP_STR_LINE_SIZE, "LEFT:  ");
				cnt = usr_in_get_enc_abs_count();
				str_buf_append_int16(app_str_line0, APP_STR_LINE_SIZE, cnt);
				disp_print(20, 80, DISP_COLOR_BLUE, DISP_COLOR_GREEN, 1, &Font_16x26, 0, app_str_line0);
				break;
			case EV_ENC_RIGHT:
				str_buf_clear(app_str_line0, APP_STR_LINE_SIZE);
				str_buf_append_string(app_str_line0, APP_STR_LINE_SIZE, "RIGHT: ");
				cnt = usr_in_get_enc_abs_count();
				str_buf_append_int16(app_str_line0, APP_STR_LINE_SIZE, cnt);
				disp_print(20, 80, DISP_COLOR_MAGENTA, DISP_COLOR_BLACK, 1, &Font_16x26, 0, app_str_line0);
				break;

		//default: // unknown event, skip
			}
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
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
	}
}

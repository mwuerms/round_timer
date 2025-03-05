/**
 * module usr_in "User Inputs"
 * Martin, 2025-02-26
 */
#include "usr_in.h"
#include "stdbool.h"
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Definitions for myTimer01 */
osTimerId_t usr_in_timer_handle;
const osTimerAttr_t usr_in_timer_attributes = {
  .name = "user_in_timer"
};

// - events, see app (receiver)
// - GPIOs ------------------------------------
#include "main.h"
// BTN0_GPIO_Port, BTN0_Pin
// BTN1_GPIO_Port, BTN1_Pin
// BTN2_GPIO_Port, BTN2_Pin
// ENC_A_GPIO_Port, ENC_A_Pin
// ENC_B_GPIO_Port, ENC_B_Pin
#define IS_BTN0_LOW() ((BTN0_GPIO_Port->IDR & BTN0_Pin) == 0)
#define IS_BTN1_LOW() ((BTN1_GPIO_Port->IDR & BTN1_Pin) == 0)
#define IS_BTN2_LOW() ((BTN2_GPIO_Port->IDR & BTN2_Pin) == 0)
#define IS_ENC_A_LOW() ((ENC_A_GPIO_Port->IDR & ENC_A_Pin) == 0)
#define IS_ENC_B_LOW() ((ENC_B_GPIO_Port->IDR & ENC_B_Pin) == 0)

int8_t usr_in_exti_task(void *argument);

osMessageQueueId_t usr_in_exti_queue_handle;
const osMessageQueueAttr_t usr_in_exti_queue_attributes = {
  .name = "usr_in_exti_queue"
};
osThreadId_t usr_in_exti_task_handle;
const osThreadAttr_t usr_in_exti_task_attributes = {
  .name = "usr_in_exti_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};


// count encoder steps, --: left, ++: right
static volatile int16_t enc_abs_count = 0; // absolute, cannot be reseted
static volatile int16_t enc_rel_count = 0; // relative, reset after reading
static volatile uint16_t usr_in_exti_event = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
	usr_in_exti_event = 0;

	// BTN0_Pin
	if(GPIO_Pin == BTN0_Pin) {
		if(IS_BTN0_LOW()) {
			// pressed
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
			usr_in_exti_event = EV_BTN0_PRESSED;
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
		}
		else {
			// released
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
			usr_in_exti_event = EV_BTN0_RELEASED;
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
		}
	}/*
	// BTN1_Pin
	if(GPIO_Pin == BTN1_Pin) {
		if(IS_BTN1_LOW()) {
			// pressed
			usr_in_exti_event = EV_BTN1_PRESSED;
		}
		else {
			// released
			usr_in_exti_event = EV_BTN1_RELEASED;
		}
	}
	// BTN2_Pin
	if(GPIO_Pin == BTN2_Pin) {
		if(IS_BTN2_LOW()) {
			// pressed
			usr_in_exti_event = EV_BTN2_PRESSED;
		}
		else {
			// released
			usr_in_exti_event = EV_BTN2_RELEASED;
		}
	}
	// ENC_A_Pin + ENC_B_Pin
	if(GPIO_Pin == ENC_A_Pin) {
		if(IS_ENC_A_LOW()) {
			if(IS_ENC_B_LOW()) {
				// left
				usr_in_exti_event = EV_ENC_LEFT;
				enc_abs_count--;
				enc_rel_count--;
			}
			else {
				// right
				usr_in_exti_event = EV_ENC_RIGHT;
				enc_abs_count++;
				enc_rel_count++;
			}
		}
		else {
			if(IS_ENC_B_LOW()) {
				// right
				usr_in_exti_event = EV_ENC_RIGHT;
				enc_abs_count++;
				enc_rel_count++;
			}
			else {
				// left
				usr_in_exti_event = EV_ENC_LEFT;
				enc_abs_count--;
				enc_rel_count--;
			}
		}
	}*/
	/*if(usr_in_exti_event) {
		osMessageQueuePut(app_ev_queue_handle, (uint16_t *)&usr_in_exti_event, 0, 0);
	}*/
	if(usr_in_exti_event) {
		osMessageQueuePut(usr_in_exti_queue_handle, (uint16_t *)&usr_in_exti_event, 0, 0);
	}
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
}

void user_in_timer_callback(void *argument) {
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
}

int8_t usr_in_exti_task(void *argument) {
	uint16_t event;
	while(1) {
		if(osMessageQueueGet(usr_in_exti_queue_handle, &event, NULL, osWaitForever) == osOK) {
			switch(event) {
			case EV_BTN0_PRESSED:
				if(IS_BTN0_LOW()) {
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
					osTimerStart(usr_in_timer_handle, 10 * portTICK_PERIOD_MS);
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
				}
				break;
			case EV_BTN0_RELEASED:
				if(!IS_BTN0_LOW()) {
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
					osTimerStart(usr_in_timer_handle, 10 * portTICK_PERIOD_MS);
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
				}
				break;
			}
		}
	}
}

void usr_in_init(void) {
	usr_in_timer_handle = osTimerNew(user_in_timer_callback, osTimerOnce, NULL, &usr_in_timer_attributes);
	usr_in_exti_queue_handle = osMessageQueueNew (16, sizeof(uint16_t), &usr_in_exti_queue_attributes);
	usr_in_exti_task_handle = osThreadNew(usr_in_exti_task, NULL, &usr_in_exti_task_attributes);

	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
	osTimerStart(usr_in_timer_handle, 25 * portTICK_PERIOD_MS);
	//usr_in_exti_event = EV_BTN0_PRESSED;
	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);


}

void usr_in_enable(uint32_t mask) {
	return;
}

void usr_in_disable(uint32_t mask) {
	return;
}

int16_t usr_in_get_enc_abs_count(void) {
	return enc_abs_count;
}

int16_t usr_in_get_enc_rel_count(void) {
	return enc_rel_count;
}

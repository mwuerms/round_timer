/**
 * module usr_in "User Inputs"
 * Martin, 2025-02-26
 */

#include "usr_in.h"
#include "stdbool.h"

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	// BTN0_Pin
	if(GPIO_Pin == BTN0_Pin) {
		if(IS_BTN0_LOW()) {
			// pressed
			global_events |= 0x0001;
		}
		else {
			// released
			global_events |= 0x0002;
		}
	}
	// BTN1_Pin
	if(GPIO_Pin == BTN1_Pin) {
		if(IS_BTN1_LOW()) {
			// pressed
			global_events |= 0x0004;
		}
		else {
			// released
			global_events |= 0x0008;
		}
	}
	// BTN2_Pin
	if(GPIO_Pin == BTN2_Pin) {
		if(IS_BTN2_LOW()) {
			// pressed
			global_events |= 0x0010;
		}
		else {
			// released
			global_events |= 0x0020;
		}
	}
	// ENC_A_Pin + ENC_B_Pin
	if(GPIO_Pin == ENC_A_Pin) {
		if(IS_ENC_A_LOW()) {
			if(IS_ENC_B_LOW()) {
				// left
				global_events |= 0x0040;
			}
			else {
				// right
				global_events |= 0x0080;
			}
		}
		else {
			if(IS_ENC_B_LOW()) {
				// right
				global_events |= 0x0080;
			}
			else {
				// left
				global_events |= 0x0040;
			}
		}
	}
}

void usr_in_init(void) {
	return;
}

void usr_in_enable(uint32_t mask) {
	return;
}

void usr_in_disable(uint32_t mask) {
	return;
}

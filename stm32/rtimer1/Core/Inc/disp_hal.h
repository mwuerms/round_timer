/**
 * Module disp_hal
 * driving display GC9A01
 * 
 * this display driver is heavily based on:
 *   https://github.com/offpic/GC9A01-DMA-TFT-STM32-STM32F401-CLOCK
 *   https://github.com/GolinskiyKonstantin/STM32_Lib_TFT_ST7789
 */

#ifndef _DISP_HAL_H_
#define _DISP_HAL_H_

#include "stdint.h"

// - public functions ---------
void disp_hal_hard_reset(void);
void disp_hal_select(void);
void disp_hal_deselect(void);
void disp_hal_send_cmd(uint8_t cmd);
void disp_hal_send_data(uint8_t data);
void disp_hal_send_data_buffer(uint8_t *data_buff, uint32_t buff_size);

#endif // _DISP_HAL_H_

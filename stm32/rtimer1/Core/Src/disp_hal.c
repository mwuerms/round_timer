/**
 * Module disp_hal
 * driving display GC9A01
 * 
 * this display driver is heavily based on:
 *   https://github.com/offpic/GC9A01-DMA-TFT-STM32-STM32F401-CLOCK
 *   https://github.com/GolinskiyKonstantin/STM32_Lib_TFT_ST7789
 */

#include "disp_hal.h"

#include "main.h"
// - GPIOs --------------------
// define these GPIOs, use standard definitions or the ones from main.h
// DISP_RST Reset to display
#ifndef DISP_RST_Pin
#define DISP_RST_Pin LCD_RST_Pin // GPIO_PIN_1
#endif
#ifndef DISP_RST_GPIO_Port
#define DISP_RST_GPIO_Port LCD_RST_GPIO_Port // GPIOA
#endif
// DISP_DC data/command line
#ifndef DISP_DC_Pin
#define DISP_DC_Pin LCD_DC_Pin // GPIO_PIN_2
#endif
#ifndef DISP_DC_GPIO_Port
#define DISP_DC_GPIO_Port LCD_DC_GPIO_Port // GPIOA
#endif
// DISP_CS SPI chip select
#ifndef DISP_CS_Pin
#define DISP_CS_Pin LCD_CS_Pin // GPIO_PIN_4
#endif
#ifndef DISP_CS_GPIO_Port
#define DISP_CS_GPIO_Port LCD_CS_GPIO_Port // GPIOA
#endif
// DISP_SCK SPI clock
#ifndef DISP_SCK_Pin
#define DISP_SCK_Pin LCD_SCK_Pin // GPIO_PIN_5
#endif
#ifndef DISP_SCK_GPIO_Port
#define DISP_SCK_GPIO_Port LCD_SCK_GPIO_Port // GPIOA
#endif
// DISP_MISO SPI data in, unused, do not read from diplay
//#define DISP_MISO_Pin LCD_MISO_Pin // GPIO_PIN_6
//#define DISP_MISO_GPIO_Port LCD_MISO_GPIO_Port // GPIOA
// DISP_MOSI SPI data out
#ifndef DISP_MOSI_Pin
#define DISP_MOSI_Pin LCD_MOSI_Pin // GPIO_PIN_7
#endif
#ifndef DISP_MOSI_GPIO_Port
#define DISP_MOSI_GPIO_Port LCD_MOSI_GPIO_Port // GPIOA
#endif

// - define SPI module
extern SPI_HandleTypeDef hspi1;
#define DISP_SPI_HAL hspi1


void disp_hal_hard_reset(void)
{
	HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(150);
}

void disp_hal_select(void)
{
	HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_RESET);
	//DISP_CS_GPIO_Port->BSRR = (DISP_CS_Pin << 16);
}

void disp_hal_deselect(void)
{
	HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_SET);
	//DISP_CS_GPIO_Port->BSRR = DISP_CS_Pin;
}

void disp_hal_send_cmd(uint8_t cmd)
{
	// pin DC LOW
	HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&DISP_SPI_HAL, &cmd, 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&DISP_SPI_HAL) != HAL_SPI_STATE_READY);

	// pin DC HIGH
	HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, GPIO_PIN_SET);
}

void disp_hal_send_data(uint8_t data)
{
	// pin DC HIGH
	HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, GPIO_PIN_SET);

	HAL_SPI_Transmit(&DISP_SPI_HAL, &data, 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&DISP_SPI_HAL) != HAL_SPI_STATE_READY);
}

void disp_hal_send_data_buffer(uint8_t *data_buff, uint32_t buff_size)
{
	// pin DC HIGH
	HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, GPIO_PIN_SET);

	HAL_SPI_Transmit(&DISP_SPI_HAL, &data_buff, buff_size, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&DISP_SPI_HAL) != HAL_SPI_STATE_READY);
}

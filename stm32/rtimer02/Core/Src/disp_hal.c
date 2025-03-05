/**
 * Module disp_hal
 * driving display GC9A01
 * 
 * this display driver is heavily based on:
 *   https://github.com/offpic/GC9A01-DMA-TFT-STM32-STM32F401-CLOCK
 *   https://github.com/GolinskiyKonstantin/STM32_Lib_TFT_ST7789
 */

#include "disp_hal.h"

// - SPI module ---------------
#include "spi.h"
#define DISP_SPI SPI2

// - GPIOs --------------------
#include "main.h"
// define these GPIOs, use standard definitions or the ones from main.h
// DISP_RST Reset to display
#ifndef DISP_RST_Pin
#define DISP_RST_Pin GPIO_PIN_10
#endif
#ifndef DISP_RST_GPIO_Port
#define DISP_RST_GPIO_Port GPIOB
#endif
#define DISP_RST_Set()    DISP_RST_GPIO_Port->BSRR = (uint32_t)DISP_RST_Pin
#define DISP_RST_Reset()  DISP_RST_GPIO_Port->BRR =  (uint32_t)DISP_RST_Pin

// DISP_DC data/command line
#ifndef DISP_DC_Pin
#define DISP_DC_Pin GPIO_PIN_11
#endif
#ifndef DISP_DC_GPIO_Port
#define DISP_DC_GPIO_Port GPIOB
#endif
#define DISP_DC_Set()    DISP_DC_GPIO_Port->BSRR = (uint32_t)DISP_DC_Pin
#define DISP_DC_Reset()  DISP_DC_GPIO_Port->BRR =  (uint32_t)DISP_DC_Pin

// DISP_CS SPI chip select
#ifndef DISP_CS_Pin
#define DISP_CS_Pin GPIO_PIN_12
#endif
#ifndef DISP_CS_GPIO_Port
#define DISP_CS_GPIO_Port GPIOB
#endif
#define DISP_CS_Set()    DISP_CS_GPIO_Port->BSRR = (uint32_t)DISP_CS_Pin
#define DISP_CS_Reset()  DISP_CS_GPIO_Port->BRR =  (uint32_t)DISP_CS_Pin
/*
// DISP_SCK SPI clock
#ifndef DISP_SCK_Pin
#define DISP_SCK_Pin GPIO_PIN_13
#endif
#ifndef DISP_SCK_GPIO_Port
#define DISP_SCK_GPIO_Port GPIOB
#endif
// DISP_MISO SPI data in, unused, do not read from diplay
//#define DISP_MISO_Pin LCD_MISO_Pin // GPIO_PIN_6
//#define DISP_MISO_GPIO_Port LCD_MISO_GPIO_Port // GPIOA
// DISP_MOSI SPI data out
#ifndef DISP_MOSI_Pin
#define DISP_MOSI_Pin GPIO_PIN_15
#endif
#ifndef DISP_MOSI_GPIO_Port
#define DISP_MOSI_GPIO_Port GPIOB
#endif
*/

void disp_hal_hard_reset(void)
{
	DISP_RST_Reset();
	//HAL_Delay(50);
	for(volatile uint32_t cnt = 0; cnt < (1*79000); cnt++); // 55 ms
	DISP_RST_Set();
	//HAL_Delay(150);
	for(volatile uint32_t cnt = 0; cnt < (3*79000); cnt++); // 55 ms

	DISP_SPI->CR1 |= SPI_CR1_SPE;
}

void disp_hal_select(void)
{
	DISP_CS_Reset();
}

void disp_hal_deselect(void)
{
	DISP_CS_Set();
}

void disp_hal_send_cmd(uint8_t cmd)
{
	// pin DC LOW
	DISP_DC_Reset();

	// thanks to https://github.com/LonelyWolf/stm32/blob/master/stm32l4-template/periph/spi.c
	/*
	do {
		while (!(SPIx->Instance->SR & SPI_SR_TXE)); // Wait until TX buffer is empty
		*((__IO uint8_t *)&SPIx->Instance->DR) = *pBuf++;
	} while (--length);
	while (SPIx->Instance->SR & SPI_SR_BSY); // Wait for the transmission of the last byte
	*/
	while((DISP_SPI->SR & SPI_FLAG_TXE) == 0); // wait until done
	*((__IO uint8_t *)&DISP_SPI->DR) = cmd; // send

	while(DISP_SPI->SR & SPI_SR_BSY); // Wait for the transmission of the last byte
}

void disp_hal_send_data(uint8_t data)
{
	// pin DC HIGH
	DISP_DC_Set();

	while((DISP_SPI->SR & SPI_FLAG_TXE) == 0); // wait until done
	*((__IO uint8_t *)&DISP_SPI->DR) = data; // send

	while(DISP_SPI->SR & SPI_SR_BSY); // Wait for the transmission of the last byte
}

void disp_hal_send_data_buffer(uint8_t *data_buff, uint32_t buff_size)
{
	// pin DC HIGH
	DISP_DC_Set();

	while(buff_size) {
		while((DISP_SPI->SR & SPI_FLAG_TXE) == 0); // wait until done
		// load only uint8_t into DR
		*((__IO uint8_t *)&DISP_SPI->DR) = *data_buff; // send
		data_buff += 1;
		buff_size -= 1;
	}
	while(DISP_SPI->SR & SPI_SR_BSY); // Wait for the transmission of the last byte
}

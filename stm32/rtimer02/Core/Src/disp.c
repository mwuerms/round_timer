/**
 * Module disp
 * driving display GC9A01
 * 
 * this display driver is heavily based on:
 *   https://github.com/offpic/GC9A01-DMA-TFT-STM32-STM32F401-CLOCK
 *   https://github.com/GolinskiyKonstantin/STM32_Lib_TFT_ST7789
 */

#include "disp.h"
#include "disp_hal.h"

uint16_t disp_width, disp_height;
uint16_t disp_x_start, disp_y_start;

static void disp_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void disp_ram_write(uint16_t *buff, uint32_t len);
static void disp_set_column(uint16_t ColumnStart, uint16_t column_end);
static void disp_set_row(uint16_t row_start, uint16_t row_end);
static void disp_draw_line_slow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

static void swap_int16_values(int16_t *pValue1, int16_t *pValue2);

void disp_init(void)
{
	disp_hal_deselect();

	//HAL_Delay(100);
	for(volatile uint32_t cnt = 0; cnt < (2*79000); cnt++); // 55 ms

	disp_width = DISP_WIDTH;
	disp_height = DISP_HEIGHT;
	disp_x_start = DISP_XSTART;
	disp_y_start = DISP_YSTART;

	disp_hal_hard_reset();

	disp_hal_select();

	disp_hal_send_cmd(DISP_InnerReg2Enable);
	disp_hal_send_cmd(0xEB);
	disp_hal_send_data(0x14);

	disp_hal_send_cmd(DISP_InnerReg1Enable);
	disp_hal_send_cmd(DISP_InnerReg2Enable);

	disp_hal_send_cmd(0xEB);
	disp_hal_send_data(0x14);

	disp_hal_send_cmd(0x84);
	disp_hal_send_data(0x40);

	disp_hal_send_cmd(0x85);
	disp_hal_send_data(0xFF);

	disp_hal_send_cmd(0x86);
	disp_hal_send_data(0xFF);

	disp_hal_send_cmd(0x87);
	disp_hal_send_data(0xFF);

	disp_hal_send_cmd(0x88);
	disp_hal_send_data(0x0A);

	disp_hal_send_cmd(0x89);
	disp_hal_send_data(0x21);

	disp_hal_send_cmd(0x8A);
	disp_hal_send_data(0x00);

	disp_hal_send_cmd(0x8B);
	disp_hal_send_data(0x80);

	disp_hal_send_cmd(0x8C);
	disp_hal_send_data(0x01);

	disp_hal_send_cmd(0x8D);
	disp_hal_send_data(0x01);

	disp_hal_send_cmd(0x8E);
	disp_hal_send_data(0xFF);

	disp_hal_send_cmd(0x8F);
	disp_hal_send_data(0xFF);

	disp_hal_send_cmd(DISP_DisplayFunctionControl);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x20); // Scan direction S360 -> S1

	// def rotation
	disp_hal_send_cmd(DISP_MADCTL);
	disp_hal_send_data(DISP_DEF_ROTATION);

	// ColorMode
	disp_hal_send_cmd(DISP_COLMOD);
	disp_hal_send_data(ColorMode_MCU_16bit & 0x77);

	disp_hal_send_cmd(0x90);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x08);

	disp_hal_send_cmd(0xBD);
	disp_hal_send_data(0x06);

	disp_hal_send_cmd(0xBC);
	disp_hal_send_data(0x00);

	disp_hal_send_cmd(0xFF);
	disp_hal_send_data(0x60);
	disp_hal_send_data(0x01);
	disp_hal_send_data(0x04);

	disp_hal_send_cmd(DISP_PWCTR2); // Power control 2
	disp_hal_send_data(0x13);		// 5.18 V
	disp_hal_send_cmd(DISP_PWCTR3); // Power control 3
	disp_hal_send_data(0x13);		// VREG2A = -3.82 V, VREG2B = 0.68 V
	disp_hal_send_cmd(DISP_PWCTR4); // Power control 4
	disp_hal_send_data(0x22);		// VREG2A = 5.88 V, VREG2B = -2.88 V

	disp_hal_send_cmd(0xBE);
	disp_hal_send_data(0x11);

	disp_hal_send_cmd(0xE1);
	disp_hal_send_data(0x10);
	disp_hal_send_data(0x0E);

	disp_hal_send_cmd(0xDF);
	disp_hal_send_data(0x21);
	disp_hal_send_data(0x0c);
	disp_hal_send_data(0x02);

	disp_hal_send_cmd(DISP_GAMMA1);
	disp_hal_send_data(0x45);
	disp_hal_send_data(0x09);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x26);
	disp_hal_send_data(0x2A);

	disp_hal_send_cmd(DISP_GAMMA2);
	disp_hal_send_data(0x43);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x72);
	disp_hal_send_data(0x36);
	disp_hal_send_data(0x37);
	disp_hal_send_data(0x6F);

	disp_hal_send_cmd(DISP_GAMMA3);
	disp_hal_send_data(0x45);
	disp_hal_send_data(0x09);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x26);
	disp_hal_send_data(0x2A);

	disp_hal_send_cmd(DISP_GAMMA4);
	disp_hal_send_data(0x43);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x72);
	disp_hal_send_data(0x36);
	disp_hal_send_data(0x37);
	disp_hal_send_data(0x6F);

	disp_hal_send_cmd(0xED);
	disp_hal_send_data(0x1B);
	disp_hal_send_data(0x0B);

	disp_hal_send_cmd(0xAE);
	disp_hal_send_data(0x77);

	disp_hal_send_cmd(0xCD);
	disp_hal_send_data(0x63);

	disp_hal_send_cmd(0x70);
	disp_hal_send_data(0x07);
	disp_hal_send_data(0x07);
	disp_hal_send_data(0x04);
	disp_hal_send_data(0x0E);
	disp_hal_send_data(0x0F);
	disp_hal_send_data(0x09);
	disp_hal_send_data(0x07);
	disp_hal_send_data(0x08);
	disp_hal_send_data(0x03);

	disp_hal_send_cmd(DISP_FRAMERATE); // Frame rate
	disp_hal_send_data(0x34);          // 4 dot inversion

	disp_hal_send_cmd(0x62);
	disp_hal_send_data(0x18);
	disp_hal_send_data(0x0D);
	disp_hal_send_data(0x71);
	disp_hal_send_data(0xED);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x18);
	disp_hal_send_data(0x0F);
	disp_hal_send_data(0x71);
	disp_hal_send_data(0xEF);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x70);

	disp_hal_send_cmd(0x63);
	disp_hal_send_data(0x18);
	disp_hal_send_data(0x11);
	disp_hal_send_data(0x71);
	disp_hal_send_data(0xF1);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x18);
	disp_hal_send_data(0x13);
	disp_hal_send_data(0x71);
	disp_hal_send_data(0xF3);
	disp_hal_send_data(0x70);
	disp_hal_send_data(0x70);

	disp_hal_send_cmd(0x64);
	disp_hal_send_data(0x28);
	disp_hal_send_data(0x29);
	disp_hal_send_data(0xF1);
	disp_hal_send_data(0x01);
	disp_hal_send_data(0xF1);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x07);

	disp_hal_send_cmd(0x66);
	disp_hal_send_data(0x3C);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0xCD);
	disp_hal_send_data(0x67);
	disp_hal_send_data(0x45);
	disp_hal_send_data(0x45);
	disp_hal_send_data(0x10);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x00);

	disp_hal_send_cmd(0x67);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x3C);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x01);
	disp_hal_send_data(0x54);
	disp_hal_send_data(0x10);
	disp_hal_send_data(0x32);
	disp_hal_send_data(0x98);

	disp_hal_send_cmd(0x74);
	disp_hal_send_data(0x10);
	disp_hal_send_data(0x85);
	disp_hal_send_data(0x80);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x00);
	disp_hal_send_data(0x4E);
	disp_hal_send_data(0x00);

	disp_hal_send_cmd(0x98);
	disp_hal_send_data(0x3e);
	disp_hal_send_data(0x07);

	disp_hal_send_cmd(DISP_TEON); // Tearing effect line on

	// Inversion Mode 1;
	disp_hal_send_cmd(DISP_INVON);

	// Sleep Mode Exit
	disp_hal_send_cmd(DISP_SLPOUT);

	//HAL_Delay(120);
	for(volatile uint32_t cnt = 0; cnt < (2*79000+30000); cnt++); // 55 ms

	// Display Power on
	disp_hal_send_cmd(DISP_DISPON);

	disp_hal_deselect();

	disp_fill_rectangle(0, 0, disp_width, disp_height, DISP_COLOR_BLACK);
}

void disp_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)
{
	if ((x >= disp_width) || (y >= disp_height))
	{
		return;
	}
	if ((x + w - 1) >= disp_width)
	{
		return;
	}
	if ((y + h - 1) >= disp_height)
	{
		return;
	}
	disp_set_window(x, y, x + w - 1, y + h - 1);
	disp_hal_select();
	disp_hal_send_data_buffer((uint8_t *)data, sizeof(uint16_t) * w * h);
	disp_hal_deselect();
}

void disp_enter_sleep_mode(void)
{
	disp_hal_select();
	disp_hal_send_cmd(DISP_SLPIN);
	disp_hal_deselect();
	//HAL_Delay(150);
	for(volatile uint32_t cnt = 0; cnt < (3*79000); cnt++); // 55 ms
}

void disp_exit_sleep_mode(void)
{
	disp_hal_select();
	disp_hal_send_cmd(DISP_SLPOUT);
	disp_hal_deselect();
	//HAL_Delay(150);
	for(volatile uint32_t cnt = 0; cnt < (3*79000); cnt++); // 55 ms
}

void disp_inversion_mode(uint8_t mode)
{
	disp_hal_select();
	if (mode)
	{
		disp_hal_send_cmd(DISP_INVON);
	}
	else
	{
		disp_hal_send_cmd(DISP_INVOFF);
	}
	disp_hal_deselect();
}

void disp_fill_screen(uint16_t color)
{
	disp_fill_rectangle(0, 0, disp_width, disp_height, color);
}

void disp_clear(void)
{
	disp_fill_rectangle(0, 0, disp_width, disp_height, 0);
}

void disp_fill_rectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

	if ((x >= disp_width) || (y >= disp_height))
	{
		return;
	}

	if ((x + w) > disp_width)
	{
		w = disp_width - x;
	}

	if ((y + h) > disp_height)
	{
		h = disp_height - y;
	}
	disp_set_window(x, y, x + w - 1, y + h - 1);
	disp_ram_write(&color, (h * w));
}

void disp_set_backlight(uint8_t value)
{
	//  if (Value > 100)
	//    Value = 100;
	//	tmr2_PWM_set(ST77xx_PWM_TMR2_Chan, Value);
}

void disp_power(uint8_t on)
{

	disp_hal_select();

	if (on)
	{
		disp_hal_send_cmd(DISP_DISPON);
	}
	else
	{
		disp_hal_send_cmd(DISP_DISPOFF);
	}

	disp_hal_deselect();
}

void disp_draw_rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

	disp_draw_line(x1, y1, x1, y2, color);
	disp_draw_line(x2, y1, x2, y2, color);
	disp_draw_line(x1, y1, x2, y1, color);
	disp_draw_line(x1, y2, x2, y2, color);
}

static void swap_int16_values(int16_t *pValue1, int16_t *pValue2)
{

	int16_t TempValue = *pValue1;
	*pValue1 = *pValue2;
	*pValue2 = TempValue;
}

void disp_draw_filled_rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t fill_color)
{

	if (x1 > x2)
	{
		swap_int16_values(&x1, &x2);
	}

	if (y1 > y2)
	{
		swap_int16_values(&y1, &y2);
	}

	disp_fill_rectangle(x1, y1, x2 - x1, y2 - y1, fill_color);
}

static void disp_draw_line_slow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

	const int16_t deltaX = abs(x2 - x1);
	const int16_t deltaY = abs(y2 - y1);
	const int16_t signX = x1 < x2 ? 1 : -1;
	const int16_t signY = y1 < y2 ? 1 : -1;

	int16_t error = deltaX - deltaY;

	disp_draw_pixel(x2, y2, color);

	while (x1 != x2 || y1 != y2)
	{

		disp_draw_pixel(x1, y1, color);
		const int16_t error2 = error * 2;

		if (error2 > -deltaY)
		{

			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{

			error += deltaX;
			y1 += signY;
		}
	}
}

void disp_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

	if (x1 == x2)
	{

		if (y1 > y2)
		{
			disp_fill_rectangle(x1, y2, 1, y1 - y2 + 1, color);
		}
		else
		{
			disp_fill_rectangle(x1, y1, 1, y2 - y1 + 1, color);
		}

		return;
	}

	if (y1 == y2)
	{

		if (x1 > x2)
		{
			disp_fill_rectangle(x2, y1, x1 - x2 + 1, 1, color);
		}
		else
		{
			disp_fill_rectangle(x1, y1, x2 - x1 + 1, 1, color);
		}

		return;
	}

	disp_draw_line_slow(x1, y1, x2, y2, color);
}

void disp_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	/* Draw lines */
	disp_draw_line(x1, y1, x2, y2, color);
	disp_draw_line(x2, y2, x3, y3, color);
	disp_draw_line(x3, y3, x1, y1, color);
}

void disp_draw_filled_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{

	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
			yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
			curpixel = 0;

	deltax = abs(x2 - x1);
	deltay = abs(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1)
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)
	{
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else
	{
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		disp_draw_line(x, y, x3, y3, color);

		num += numadd;
		if (num >= den)
		{
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void disp_draw_pixel(int16_t x, int16_t y, uint16_t color)
{

	if ((x < 0) || (x >= disp_width) || (y < 0) || (y >= disp_height))
	{
		return;
	}

#if FRAME_BUFFER // если включен буфер кадра
	buff_frame[y * disp_width + x] = ((color & 0xFF) << 8) | (color >> 8);
#else // если попиксельный вывод
	disp_set_window(x, y, x, y);
	disp_ram_write(&color, 1);
#endif
}

void disp_draw_filled_circle(int16_t x0, int16_t y0, int16_t radius, uint16_t fill_color)
{

	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;

	while (y >= 0)
	{

		disp_draw_line(x0 + x, y0 - y, x0 + x, y0 + y, fill_color);
		disp_draw_line(x0 - x, y0 - y, x0 - x, y0 + y, fill_color);
		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0)
		{

			++x;
			delta += 2 * x + 1;
			continue;
		}

		error = 2 * (delta - x) - 1;

		if (delta > 0 && error > 0)
		{

			--y;
			delta += 1 - 2 * y;
			continue;
		}

		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void disp_draw_circle(int16_t x0, int16_t y0, int16_t radius, uint16_t color)
{

	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;

	while (y >= 0)
	{

		disp_draw_pixel(x0 + x, y0 + y, color);
		disp_draw_pixel(x0 + x, y0 - y, color);
		disp_draw_pixel(x0 - x, y0 + y, color);
		disp_draw_pixel(x0 - x, y0 - y, color);
		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0)
		{

			++x;
			delta += 2 * x + 1;
			continue;
		}

		error = 2 * (delta - x) - 1;

		if (delta > 0 && error > 0)
		{

			--y;
			delta += 1 - 2 * y;
			continue;
		}

		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void disp_draw_char(uint16_t x, uint16_t y, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font, uint8_t multiplier, unsigned char ch)
{

	uint32_t i, b, j;
	uint32_t x_pos = x, y_pos = y;
	uint8_t xx, yy;

	if (multiplier < 1)
	{
		multiplier = 1;
	}

	/* Check available space in LCD */
	if (disp_width >= (x + font->FontWidth) || disp_height >= (y + font->FontHeight))
	{

		/* Go through font */
		for (i = 0; i < font->FontHeight; i++)
		{

			if (ch < 127)
			{
				b = font->data[(ch - 32) * font->FontHeight + i];
			}

			else if ((uint8_t)ch > 191)
			{
				// +96 это так как латинские символы и знаки в шрифтах занимают 96 позиций
				// и если в шрифте который содержит сперва латиницу и спец символы и потом
				// только кирилицу то нужно добавлять 95 если шрифт
				// содержит только кирилицу то +96 не нужно
				b = font->data[((ch - 192) + 96) * font->FontHeight + i];
			}

			else if ((uint8_t)ch == 168)
			{ // 168 символ по ASCII - Ё
				// 160 эллемент ( символ Ё )
				b = font->data[(160) * font->FontHeight + i];
			}

			else if ((uint8_t)ch == 184)
			{ // 184 символ по ASCII - ё
				// 161 эллемент  ( символ ё )
				b = font->data[(161) * font->FontHeight + i];
			}
			//-------------------------------------------------------------------

			//----  Украинская раскладка ----------------------------------------------------
			else if ((uint8_t)ch == 170)
			{ // 168 символ по ASCII - Є
				// 162 эллемент ( символ Є )
				b = font->data[(162) * font->FontHeight + i];
			}
			else if ((uint8_t)ch == 175)
			{ // 184 символ по ASCII - Ї
				// 163 эллемент  ( символ Ї )
				b = font->data[(163) * font->FontHeight + i];
			}
			else if ((uint8_t)ch == 178)
			{ // 168 символ по ASCII - І
				// 164 эллемент ( символ І )
				b = font->data[(164) * font->FontHeight + i];
			}
			else if ((uint8_t)ch == 179)
			{ // 184 символ по ASCII - і
				// 165 эллемент  ( символ і )
				b = font->data[(165) * font->FontHeight + i];
			}
			else if ((uint8_t)ch == 186)
			{ // 184 символ по ASCII - є
				// 166 эллемент  ( символ є )
				b = font->data[(166) * font->FontHeight + i];
			}
			else if ((uint8_t)ch == 191)
			{ // 168 символ по ASCII - ї
				// 167 эллемент ( символ ї )
				b = font->data[(167) * font->FontHeight + i];
			}
			//-----------------------------------------------------------------------------

			for (j = 0; j < font->FontWidth; j++)
			{

				if ((b << j) & 0x8000)
				{

					for (yy = 0; yy < multiplier; yy++)
					{
						for (xx = 0; xx < multiplier; xx++)
						{
							disp_draw_pixel(x_pos + xx, y_pos + yy, text_color);
						}
					}
				}
				else if (transparent_bg)
				{

					for (yy = 0; yy < multiplier; yy++)
					{
						for (xx = 0; xx < multiplier; xx++)
						{
							disp_draw_pixel(x_pos + xx, y_pos + yy, bg_color);
						}
					}
				}
				x_pos = x_pos + multiplier;
			}
			x_pos = x;
			y_pos = y_pos + multiplier;
		}
	}
}

void disp_print(uint16_t x, uint16_t y, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font, uint8_t multiplier, char *str)
{

	if (multiplier < 1)
	{
		multiplier = 1;
	}

	unsigned char buff_char;

	uint16_t len = strlen(str);

	while (len--)
	{

		//---------------------------------------------------------------------
		// проверка на кириллицу UTF-8, если латиница то пропускаем if
		// Расширенные символы ASCII Win-1251 кириллица (код символа 128-255)
		// проверяем первый байт из двух ( так как UTF-8 ето два байта )
		// если он больше либо равен 0xC0 ( первый байт в кириллеце будет равен 0xD0 либо 0xD1 именно в алфавите )
		if ((uint8_t)*str >= 0xC0)
		{ // код 0xC0 соответствует символу кириллица 'A' по ASCII Win-1251

			// проверяем какой именно байт первый 0xD0 либо 0xD1---------------------------------------------
			switch ((uint8_t)*str)
			{
			case 0xD0:
			{
				// увеличиваем массив так как нам нужен второй байт
				str++;
				// проверяем второй байт там сам символ
				if ((uint8_t)*str >= 0x90 && (uint8_t)*str <= 0xBF)
				{
					buff_char = (*str) + 0x30;
				} // байт символов А...Я а...п  делаем здвиг на +48
				else if ((uint8_t)*str == 0x81)
				{
					buff_char = 0xA8;
					break;
				} // байт символа Ё ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				else if ((uint8_t)*str == 0x84)
				{
					buff_char = 0xAA;
					break;
				} // байт символа Є ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				else if ((uint8_t)*str == 0x86)
				{
					buff_char = 0xB2;
					break;
				} // байт символа І ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				else if ((uint8_t)*str == 0x87)
				{
					buff_char = 0xAF;
					break;
				} // байт символа Ї ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				break;
			}
			case 0xD1:
			{
				// увеличиваем массив так как нам нужен второй байт
				str++;
				// проверяем второй байт там сам символ
				if ((uint8_t)*str >= 0x80 && (uint8_t)*str <= 0x8F)
				{
					buff_char = (*str) + 0x70;
				} // байт символов п...я	елаем здвиг на +112
				else if ((uint8_t)*str == 0x91)
				{
					buff_char = 0xB8;
					break;
				} // байт символа ё ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				else if ((uint8_t)*str == 0x94)
				{
					buff_char = 0xBA;
					break;
				} // байт символа є ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				else if ((uint8_t)*str == 0x96)
				{
					buff_char = 0xB3;
					break;
				} // байт символа і ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				else if ((uint8_t)*str == 0x97)
				{
					buff_char = 0xBF;
					break;
				} // байт символа ї ( если нужнф еще символы добавляем тут и в функции DrawChar() )
				break;
			}
			}
			//------------------------------------------------------------------------------------------------
			// уменьшаем еще переменную так как израсходывали 2 байта для кириллицы
			len--;

			disp_draw_char(x, y, text_color, bg_color, transparent_bg, font, multiplier, buff_char);
		}
		//---------------------------------------------------------------------
		else
		{
			disp_draw_char(x, y, text_color, bg_color, transparent_bg, font, multiplier, *str);
		}

		x = x + (font->FontWidth * multiplier);
		/* Increase string pointer */
		str++;
	}
}

void disp_rotation(uint8_t rotation, uint8_t vert_mirror, uint8_t horiz_mirror)
{
	disp_hal_select();

	uint8_t Value;
	rotation &= 7;

	disp_hal_send_cmd(DISP_MADCTL);

	switch (rotation)
	{
	case 0:
		Value = 0;
		break;
	case 1:
		Value = DISP_MADCTL_MX;
		break;
	case 2:
		Value = DISP_MADCTL_MY;
		break;
	case 3:
		Value = DISP_MADCTL_MX | DISP_MADCTL_MY;
		break;
	case 4:
		Value = DISP_MADCTL_MV;
		break;
	case 5:
		Value = DISP_MADCTL_MV | DISP_MADCTL_MX;
		break;
	case 6:
		Value = DISP_MADCTL_MV | DISP_MADCTL_MY;
		break;
	case 7:
		Value = DISP_MADCTL_MV | DISP_MADCTL_MX | DISP_MADCTL_MY;
		break;
	}

	if (vert_mirror)
	{
		Value |= DISP_MADCTL_ML;
	}

	if (horiz_mirror)
	{
		Value |= DISP_MADCTL_MH;
	}

	// RGB or BGR
	Value |= DISP_DEF_ROTATION;

	disp_hal_send_data(Value);
	disp_hal_deselect();
}

void disp_draw_bitmap(int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h, uint16_t color)
{
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for (int16_t j = 0; j < h; j++, y++)
	{

		for (int16_t i = 0; i < w; i++)
		{

			if (i & 7)
			{
				byte <<= 1;
			}
			else
			{
				byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
			}

			if (byte & 0x80)
			{
				disp_draw_pixel(x + i, y, color);
			}
		}
	}
}

void disp_draw_filled_round_rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t corner_radius, uint16_t color)
{
	int16_t max_radius = ((width < height) ? width : height) / 2; // 1/2 minor axis
	if (corner_radius > max_radius)
	{
		corner_radius = max_radius;
	}

	disp_draw_filled_rectangle(x + corner_radius, y, x + corner_radius + width - 2 * corner_radius, y + height, color);
	// draw four corners
	disp_draw_filled_circle_helper(x + width - corner_radius - 1, y + corner_radius, corner_radius, 1, height - 2 * corner_radius - 1, color);
	disp_draw_filled_circle_helper(x + corner_radius, y + corner_radius, corner_radius, 2, height - 2 * corner_radius - 1, color);
}

void disp_draw_filled_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	int16_t px = x;
	int16_t py = y;

	delta++; // Avoid some +1's in the loop

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (x < (y + 1))
		{
			if (corners & 1)
			{
				disp_draw_line(x0 + x, y0 - y, x0 + x, y0 - y - 1 + 2 * y + delta, color);
			}
			if (corners & 2)
			{
				disp_draw_line(x0 - x, y0 - y, x0 - x, y0 - y - 1 + 2 * y + delta, color);
			}
		}
		if (y != py)
		{
			if (corners & 1)
			{
				disp_draw_line(x0 + py, y0 - px, x0 + py, y0 - px - 1 + 2 * px + delta, color);
			}
			if (corners & 2)
			{
				disp_draw_line(x0 - py, y0 - px, x0 - py, y0 - px - 1 + 2 * px + delta, color);
			}
			py = y;
		}
		px = x;
	}
}

void disp_draw_circle_helper(int16_t x0, int16_t y0, int16_t radius, int8_t quadrant_mask, uint16_t color)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	while (x <= y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		if (quadrant_mask & 0x4)
		{
			disp_draw_pixel(x0 + x, y0 + y, color);
			disp_draw_pixel(x0 + y, y0 + x, color);
		}
		if (quadrant_mask & 0x2)
		{
			disp_draw_pixel(x0 + x, y0 - y, color);
			disp_draw_pixel(x0 + y, y0 - x, color);
		}
		if (quadrant_mask & 0x8)
		{
			disp_draw_pixel(x0 - y, y0 + x, color);
			disp_draw_pixel(x0 - x, y0 + y, color);
		}
		if (quadrant_mask & 0x1)
		{
			disp_draw_pixel(x0 - y, y0 - x, color);
			disp_draw_pixel(x0 - x, y0 - y, color);
		}
	}
}

void disp_draw_round_rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, int16_t corner_radius, uint16_t color)
{

	int16_t max_radius = ((width < height) ? width : height) / 2; // 1/2 minor axis
	if (corner_radius > max_radius)
	{
		corner_radius = max_radius;
	}

	disp_draw_line(x + corner_radius, y, x + corner_radius + width - 1 - 2 * corner_radius, y, color);						   // Top
	disp_draw_line(x + corner_radius, y + height - 1, x + corner_radius + width - 1 - 2 * corner_radius, y + height - 1, color); // Bottom
	disp_draw_line(x, y + corner_radius, x, y + corner_radius + height - 1 - 2 * corner_radius, color);						   // Left
	disp_draw_line(x + width - 1, y + corner_radius, x + width - 1, y + corner_radius + height - 1 - 2 * corner_radius, color);  // Right

	// draw four corners
	disp_draw_circle_helper(x + corner_radius, y + corner_radius, corner_radius, 1, color);
	disp_draw_circle_helper(x + width - corner_radius - 1, y + corner_radius, corner_radius, 2, color);
	disp_draw_circle_helper(x + width - corner_radius - 1, y + height - corner_radius - 1, corner_radius, 4, color);
	disp_draw_circle_helper(x + corner_radius, y + height - corner_radius - 1, corner_radius, 8, color);
}

void disp_draw_thick_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, uint8_t thick)
{
	const int16_t deltaX = abs(x2 - x1);
	const int16_t deltaY = abs(y2 - y1);
	const int16_t signX = x1 < x2 ? 1 : -1;
	const int16_t signY = y1 < y2 ? 1 : -1;

	int16_t error = deltaX - deltaY;

	if (thick > 1)
	{
		disp_draw_filled_circle(x2, y2, thick >> 1, color);
	}
	else
	{
		disp_draw_pixel(x2, y2, color);
	}

	while (x1 != x2 || y1 != y2)
	{
		if (thick > 1)
		{
			disp_draw_filled_circle(x1, y1, thick >> 1, color);
		}
		else
		{
			disp_draw_pixel(x1, y1, color);
		}

		const int16_t error2 = error * 2;
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void disp_draw_arc(int16_t x0, int16_t y0, int16_t radius, int16_t start_angle, int16_t end_angle, uint16_t color, uint8_t thick)
{
	int16_t x_last = -1, y_last = -1;
	start_angle -= 90;
	end_angle -= 90;

	for (int16_t angle = start_angle; angle <= end_angle; angle += 2)
	{
		float angleRad = (float)angle * PI / 180;
		int x = cos(angleRad) * radius + x0;
		int y = sin(angleRad) * radius + y0;

		if (x_last == -1 || y_last == -1)
		{
			x_last = x;
			y_last = y;
			continue;
		}

		if (thick > 1)
		{
			disp_draw_thick_line(x_last, y_last, x, y, color, thick);
		}
		else
		{
			disp_draw_line(x_last, y_last, x, y, color);
		}

		x_last = x;
		y_last = y;
	}
}

// - private functions ---------------------------------------------------------
static void disp_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

	disp_hal_select();

	disp_set_column(x0, x1);
	disp_set_row(y0, y1);

	// write to RAM
	disp_hal_send_cmd(DISP_RAMWR);

	disp_hal_deselect();
}

static void disp_ram_write(uint16_t *pBuff, uint32_t Len)
{

	disp_hal_select();

	uint8_t buff[2];
	buff[0] = *pBuff >> 8;
	buff[1] = *pBuff & 0xFF;

	while (Len--)
	{
		disp_hal_send_data_buffer((uint8_t *)buff, 2);
	}

	disp_hal_deselect();
}

static void disp_set_column(uint16_t column_start, uint16_t column_end)
{

	if (column_start > column_end)
	{
		return;
	}

	if (column_end > disp_width)
	{
		return;
	}

	column_start += disp_x_start;
	column_end += disp_x_start;

	disp_hal_send_cmd(DISP_CASET);
	disp_hal_send_data(column_start >> 8);
	disp_hal_send_data(column_start & 0xFF);
	disp_hal_send_data(column_end >> 8);
	disp_hal_send_data(column_end & 0xFF);
}

static void disp_set_row(uint16_t row_start, uint16_t row_end)
{

	if (row_start > row_end)
	{
		return;
	}

	if (row_end > disp_height)
	{
		return;
	}

	row_start += disp_y_start;
	row_end += disp_y_start;

	disp_hal_send_cmd(DISP_RASET);
	disp_hal_send_data(row_start >> 8);
	disp_hal_send_data(row_start & 0xFF);
	disp_hal_send_data(row_end >> 8);
	disp_hal_send_data(row_end & 0xFF);
}

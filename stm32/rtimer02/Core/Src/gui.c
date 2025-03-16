/**
 * module gui "GUI elements on display"
 * Martin, 2025-03-11
 */

#include "gui.h"

void gui_element_set_redraw(gui_element_t *g) {
	if(g) {
		g->flags |= GUI_ELEMENT_FLAG_REDRAW;
	}
}

void gui_element_show_border(gui_element_t *g) {
	if(g) {
		g->flags |= GUI_ELEMENT_FLAG_REDRAW | GUI_ELEMENT_FLAG_SHOW_BORDER;
	}
}

void gui_element_show_no_border(gui_element_t *g) {
	if(g) {
		g->flags &= ~GUI_ELEMENT_FLAG_SHOW_BORDER;
		g->flags |=  GUI_ELEMENT_FLAG_REDRAW;
	}
}

void gui_draw_element(gui_element_t *g) {
	if(g == NULL) {
		// error
		return;
	}
	if((g->flags & GUI_ELEMENT_FLAG_REDRAW) == 0 ) {
		// nothing to do, skip
		return;
	}
	g->flags &= ~GUI_ELEMENT_FLAG_REDRAW;

	switch(g->type) {
	case GUI_ELEMENT_TYPE_TEXT_LABEL:
		gui_clear_area(g->x1, g->y1, g->size_x1, g->size_y1, g->bg_color);
		gui_text(g->content.text.str, g->x0, g->y0, g->fg_color, g->bg_color, g->flags & GUI_ELEMENT_FLAG_TRANSPARENT_BG, g->content.text.font);
		break;
	case GUI_ELEMENT_TYPE_PROGRESS_BAR:
		gui_clear_area(g->x1, g->y1, g->size_x1, g->size_y1, g->bg_color);
		gui_progress_bar(g->content.progress, g->x0, g->y0, g->size_x, g->size_y, g->border, g->fg_color);
		break;
	case GUI_ELEMENT_TYPE_SYMBOL:
		gui_clear_area(g->x1, g->y1, g->size_x1, g->size_y1, g->bg_color);
		gui_draw_symbol(g->content.symbol, g->x0, g->y0, g->size_x, g->size_y, 0, g->fg_color, g->bg_color, g->flags & GUI_ELEMENT_FLAG_TRANSPARENT_BG);
		if(g->flags & GUI_ELEMENT_FLAG_SHOW_BORDER) {
			gui_cursor(g->x0, g->y0, g->size_x, g->size_y, g->border, g->fg_color);
		}
		break;
	case GUI_ELEMENT_TYPE_TEXT_SYMBOL:
		gui_clear_area(g->x1, g->y1, g->size_x1, g->size_y1, g->bg_color);
		gui_text(g->content.text.str, g->x0, g->y0, g->fg_color, g->bg_color, g->flags & GUI_ELEMENT_FLAG_TRANSPARENT_BG, g->content.text.font);
		uint16_t fwid = (uint16_t)(g->content.text.font->FontWidth);
		uint16_t nb_chars = (uint16_t)strlen(g->content.text.str);
		//gui_draw_symbol(g->content.symbol, g->x0+fwid*nb_chars+4, g->y0, g->size_x, g->size_y, 0, g->fg_color, g->bg_color, g->flags & GUI_ELEMENT_FLAG_TRANSPARENT_BG);
		gui_draw_symbol(g->content.symbol, g->x0+fwid*nb_chars+4, g->y0, 16, 16, 0, g->fg_color, g->bg_color, g->flags & GUI_ELEMENT_FLAG_TRANSPARENT_BG);
		if(g->flags & GUI_ELEMENT_FLAG_SHOW_BORDER) {
			gui_cursor(g->x0, g->y0, g->size_x, g->size_y, g->border, g->fg_color);
		}
		break;
	}
}

void gui_draw_position_cross(uint16_t x0, uint16_t y0, uint16_t color) {
	disp_draw_line(x0-2, y0, x0+2, y0, color);
	disp_draw_line(x0, y0-2, x0, y0+2, color);
}

void gui_text(char *text, uint16_t x0, uint16_t y0, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	// some checks first
	if((x0 > DISP_WIDTH) || (y0 > DISP_HEIGHT)){
		// error, outside display
		return;
	}
	if(text == NULL) {
		// error, no text
		return;
	}
	disp_print(x0, y0, text_color, bg_color, transparent_bg, font, 0, text);
}

void gui_text_with_center_x(char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	uint16_t fwid, nb_chars;
	// some checks first
	if((x0 > DISP_WIDTH) || (y0 > DISP_HEIGHT)){
		// error, outside display
		return;
	}
	if(text == NULL) {
		// error, no text
		return;
	}
	fwid = (uint16_t)font->FontWidth;
	//fhi  = (uint16_t)font->FontHeight;
	nb_chars = (uint16_t)strlen(text)-1;
	if(center_x) {
		// center text
		x0 = DISP_CENTER_X - (fwid * nb_chars)/2 - fwid/2;
	}
	disp_print(x0, y0, text_color, bg_color, transparent_bg, font, 0, text);
}

void gui_element_init_text_label(gui_element_t *g, char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	uint16_t fwid, nb_chars;
	g->type = GUI_ELEMENT_TYPE_TEXT_LABEL;
	g->flags = 0; // clear all
	g->fg_color = color;
	g->bg_color = bg_color;
	if(transparent_bg) {
		g->flags |= GUI_ELEMENT_FLAG_TRANSPARENT_BG;
	}
	strcpy(g->content.text.str, text); // deep copy
	fwid = (uint16_t)font->FontWidth;
	nb_chars = (uint16_t)strlen(text)-1;
	g->size_x = (fwid * nb_chars) + fwid;
	g->size_y = (uint16_t)font->FontHeight;
	g->content.text.flags = 0;
	if(center_x) {
		g->content.text.flags = GUI_ELEMENT_TEXT_FLAG_CENTER;
		x0 = DISP_CENTER_X - (g->size_x)/2;
	}
	g->x0 = x0;
	g->y0 = y0;
	g->border = 0;
	g->x1 = g->x0-g->border;
	g->y1 = g->y0-g->border;
	g->size_x1 = g->size_x+2*g->border;
	g->size_y1 = g->size_y+2*g->border;
	//g->x2 = g->x0+g->size_x+g->border;
	//g->y2 = g->y0+g->size_y+g->border;
	g->content.text.font = font;
}

void gui_text_box(char *text, uint16_t x0, uint16_t y0, uint8_t center_x, uint16_t space, uint16_t text_color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	uint16_t fwid, fhi, nb_chars;
	// some checks first
	if((x0 > DISP_WIDTH) || (y0 > DISP_HEIGHT)){
		// error, outside display
		return;
	}
	if(text == NULL) {
		// error, no text
		return;
	}
	fwid = (uint16_t)font->FontWidth;
	fhi  = (uint16_t)font->FontHeight;
	nb_chars = (uint16_t)strlen(text)-1;
	if(center_x) {
		// center text
		x0 = DISP_CENTER_X - (fwid * nb_chars)/2 - fwid/2;
	}
	disp_draw_round_rectangle(x0-space, y0-space, (fwid * (nb_chars+1))+2*space, fhi+2*space, space, text_color);
	disp_print(x0, y0, text_color, bg_color, transparent_bg, font, 0, text);
}

void gui_progress_bar(uint16_t progress, uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint16_t space, uint16_t color) {
	uint16_t x1;
	if(progress > 100) {
		progress = 100;
	}
	// border
	disp_draw_round_rectangle(x0-space, y0-space, size_x+2*space, size_y+2*space, 2*space, color);
	x1 = x0 + size_x;
	size_x = (size_x * progress)/100;
	x0 = x1 - size_x; // right aligned
	disp_draw_filled_round_rectangle(x0, y0, size_x, size_y, space, color);
}

void gui_element_init_progress_bar(gui_element_t *g, uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint16_t color, uint16_t bg_color, uint8_t transparent_bg) {
	g->type = GUI_ELEMENT_TYPE_PROGRESS_BAR;
	g->flags = 0; // clear all
	g->x0 = x0;
	g->y0 = y0;
	g->size_x = size_x;
	g->size_y = size_y;
	g->fg_color = color;
	g->bg_color = bg_color;
	if(transparent_bg) {
		g->flags |= GUI_ELEMENT_FLAG_TRANSPARENT_BG;
	}
	g->flags |= GUI_ELEMENT_FLAG_SHOW_BORDER;
	g->border = 4;
	g->x1 = g->x0-g->border;
	g->y1 = g->y0-g->border;
	g->size_x1 = g->size_x+2*g->border+2;
	g->size_y1 = g->size_y+2*g->border+2;
	g->content.progress = 0;
}

void gui_element_set_progress(gui_element_t *g, uint16_t progress) {
	if(g) {
		g->flags |= GUI_ELEMENT_FLAG_REDRAW | GUI_ELEMENT_FLAG_SHOW_BORDER;
		g->content.progress = progress;
	}
}

// GUI_SYMBOL_STOP_SIZE_16x16 (0)
// GUI_SYMBOL_STOP_SIZE_32x32 (1)
// GUI_SYMBOL_PLAY_SIZE_16x16 (2)
// GUI_SYMBOL_PLAY_SIZE_32x32 (3)
// GUI_SYMBOL_PAUSE_SIZE_16x16 (4)
// GUI_SYMBOL_PAUSE_SIZE_32x32 (5)
// GUI_SYMBOL_ALARM_SIZE_16x16 (6)
// GUI_SYMBOL_ALARM_SIZE_32x32 (7)
static inline void gui_draw_stop_symbol(uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint16_t corner, uint16_t color) {
	disp_draw_filled_round_rectangle(x0, y0, size_x, size_y, corner, color);
}

static inline void gui_draw_pause_symbol(uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint16_t corner, uint16_t color) {
	x0 = x0+size_x/8+0*size_x/4;
	disp_draw_filled_round_rectangle(x0, y0, size_x/4, size_y, corner, color);
	x0 = x0+2*size_x/4;
	disp_draw_filled_round_rectangle(x0, y0, size_x/4, size_y, corner, color);
}

void gui_draw_symbol(uint16_t symbol, uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint8_t center_x, uint16_t color, uint16_t bg_color, uint8_t transparent_bg) {
	switch(symbol) {
	case GUI_SYMBOL_STOP:
		if(center_x) {
			x0 = DISP_CENTER_X - size_x/2;
		}
		gui_draw_stop_symbol(x0, y0, size_x, size_y, 2, color);
		break;
	case GUI_SYMBOL_PLAY:
		if(center_x) {
			x0 = x0 - size_x/2;
		}
		disp_draw_filled_triangle(x0, y0, x0+size_x, y0+size_y/2, x0, y0+size_y, color);
		break;
	case GUI_SYMBOL_PAUSE:
		if(center_x) {
			x0 = DISP_CENTER_X - size_x/2;
		}
		gui_draw_pause_symbol(x0, y0, size_x, size_y, 2, color);
		break;
	case GUI_SYMBOL_ALARM:
		if(center_x) {
			x0 = x0 - size_x/2;
		}
		disp_draw_circle(x0+size_x/2, y0+size_y/2, size_x/2-6, color);
		break;
	}
}

void gui_element_init_small_text_symbol(gui_element_t *g, char *text, uint16_t symbol, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t transparent_bg, FontDef_t *font) {
	uint16_t fwid, nb_chars;
	g->type = GUI_ELEMENT_TYPE_TEXT_SYMBOL;
	g->flags = 0; // clear all
	g->x0 = x0;
	g->y0 = y0;
	g->size_x = 16; // small
	g->size_y = 16;
	g->fg_color = color;
	g->bg_color = bg_color;
	if(transparent_bg) {
		g->flags |= GUI_ELEMENT_FLAG_TRANSPARENT_BG;
	}
	strcpy(g->content.text.str, text); // deep copy
	fwid = (uint16_t)font->FontWidth;
	nb_chars = (uint16_t)strlen(text)-1;
	g->size_x = (fwid * nb_chars) + fwid+4+16+2;
	g->size_y = (uint16_t)font->FontHeight;
	g->content.text.flags = 0; // no center here
	g->x0 = x0;
	g->y0 = y0;
	//default g->flags |= GUI_ELEMENT_FLAG_SHOW_BORDER;
	g->border = 4;
	g->x1 = g->x0-g->border;
	g->y1 = g->y0-g->border;
	g->size_x1 = g->size_x+2*g->border+2;
	g->size_y1 = g->size_y+2*g->border+2;
	//g->x2 = g->x0+g->size_x+g->border+2;
	//g->y2 = g->y0+g->size_y+g->border+2;
	g->content.text.font = font;
	g->content.symbol = symbol;
}

void gui_element_init_large_symbol(gui_element_t *g, uint16_t symbol, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t transparent_bg) {
	g->type = GUI_ELEMENT_TYPE_SYMBOL;
	g->flags = 0; // clear all
	g->x0 = x0;
	g->y0 = y0;
	g->size_x = 32; // large
	g->size_y = 32;
	g->fg_color = color;
	g->bg_color = bg_color;
	if(transparent_bg) {
		g->flags |= GUI_ELEMENT_FLAG_TRANSPARENT_BG;
	}
	g->size_x = 32;
	g->size_y = 32;
	g->content.text.flags = 0; // no center here
	g->x0 = x0;
	g->y0 = y0;
	//default g->flags |= GUI_ELEMENT_FLAG_SHOW_BORDER;
	g->border = 4;
	g->x1 = g->x0-g->border-2;
	g->y1 = g->y0-g->border-2;
	g->size_x1 = g->size_x+2*g->border+4;
	g->size_y1 = g->size_y+2*g->border+4;
	g->content.symbol = symbol;
}

void gui_element_set_symbol(gui_element_t *g, uint16_t symbol) {
	if(g) {
		g->flags |= GUI_ELEMENT_FLAG_REDRAW;
		g->content.symbol = symbol;
	}
}

void gui_box(uint16_t x0, uint16_t y0, uint16_t size_x, uint16_t size_y, uint16_t space, uint16_t color) {
	disp_draw_round_rectangle(x0-space, y0-space, size_x+2*space, size_y+2*space, space, color);
}

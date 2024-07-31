#include <Windows.h>
#include <stdint.h>
#include <string.h>
#include "font.h"
#include "colors.h"

static BITMAPINFO bitmap_info;
static void* bitmap_memory;

static const int BITMAP_WIDTH = 400;
static const int BITMAP_HEIGHT = 300;
static const int BITMAP_BYTES_PER_PIXEL = 4;
static const int BITMAP_PITCH = BITMAP_WIDTH * BITMAP_BYTES_PER_PIXEL;
static const size_t BITMAP_MEMORY_SIZE = BITMAP_BYTES_PER_PIXEL * BITMAP_WIDTH * BITMAP_HEIGHT;
static const double BITMAP_RATIO = (double)(BITMAP_HEIGHT)/(double)(BITMAP_WIDTH);

static int draw_rect_x;
static int draw_rect_y;
static int draw_rect_width;
static int draw_rect_height;

#define TILE_SIZE 14

static inline void win32_init_dib_section()
{
	if (bitmap_memory)
	{
		VirtualFree(bitmap_memory, 0, MEM_RELEASE);
	}

	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = BITMAP_WIDTH;
	bitmap_info.bmiHeader.biHeight = -BITMAP_HEIGHT;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;

	bitmap_memory = VirtualAlloc(0, BITMAP_MEMORY_SIZE, MEM_COMMIT, PAGE_READWRITE);
}

static inline void win32_update_draw_rect(RECT* window_rect)
{
	int window_width = window_rect->right - window_rect->left;
	int window_height = window_rect->bottom - window_rect->top;

	double window_ratio = ((double)window_width) / ((double)window_height);

	// this is a mess
	draw_rect_width = (window_ratio > BITMAP_RATIO) ?
		(int)(double) ( (double)(BITMAP_WIDTH) *
						(double)((double)(window_height)/(double)(BITMAP_HEIGHT)))
		: window_width;

	draw_rect_height = (window_ratio > BITMAP_RATIO) ? window_height
		: (int) (double) ( (double)(BITMAP_HEIGHT) * (double)((double)(window_width) / (double)(BITMAP_WIDTH)));

	draw_rect_y = (window_height - draw_rect_height) / 2;
	draw_rect_x = (window_width - draw_rect_width) / 2;
}

static inline void win32_update_window(HDC device_context)
{
	StretchDIBits(device_context,
				  draw_rect_x, draw_rect_y,          // Dest
				  draw_rect_width, draw_rect_height, // Dest
				  0, 0, BITMAP_WIDTH, BITMAP_HEIGHT, // Src
				  bitmap_memory,
				  &bitmap_info,
				  DIB_RGB_COLORS,
				  SRCCOPY);
}

static inline void clear_screen()
{
	memset(bitmap_memory, 0x28, BITMAP_MEMORY_SIZE);
}

static inline void draw_quad(int x, int y, int width, int height, uint8_t color)
{
	uint8_t* top_left = (uint8_t*)bitmap_memory + ((y * BITMAP_PITCH) + (x * 4));

	for (int yy = 0; yy < height; yy++)
	{
		uint8_t* row = top_left + (yy * BITMAP_PITCH);
		memset(row, color, sizeof(uint32_t) * width);
	}
}

static inline void draw_tile(int x, int y, tile color)
{
	uint32_t bright_color = bright_tile_color(color);
	uint32_t mid_color = mid_tile_color(color);
	uint32_t dark_color = dark_tile_color(color);
	
	for (int yy = 0; yy < TILE_SIZE; yy++)
	{
		uint8_t* pixel = (uint8_t*)bitmap_memory + (BITMAP_PITCH * (y + yy) );
		for (int xx = 0; xx < TILE_SIZE; xx++)
		{
			uint32_t color = mid_color;
			if (xx <= 1 || yy <= 1)
			{
				color = bright_color;
			}
			else if (xx >= TILE_SIZE-1 || yy >= TILE_SIZE-1)
			{
				color = dark_color;
			}
			
			*(uint32_t*)(pixel + ((x + xx) * 4) ) = color;
		}
	}
}


static inline void draw_pixel(int x, int y, uint32_t color)
{
	*(uint32_t*)((uint8_t*)bitmap_memory + (y * BITMAP_PITCH) + (x * 4)) = color;
}

static inline void render_char(char c, int x, int y, uint32_t color)
{
	char* glyph = font_glyphs[c];

	for (int yy = 0; yy < 8; yy++)
	{
		for (int xx = 0; xx < 8; xx++)
		{
			if (glyph[yy] & (1 << xx))
			{
				draw_pixel(x + xx, y + yy, color);
			}
		}
	}
}

static inline void render_text(int x, int y, const char* text, uint32_t color)
{
	int counter = 0;
	while (*text)
	{
		render_char(*text, x + (counter * 8), y, color);
		counter++;
		text++;
	}
}

static inline void render_text_overlay(int x, int y, const char* text, uint32_t back_color, uint32_t front_color)
{
	render_text(x+1, y+1, text, back_color);
	render_text(x, y, text, front_color);
}

static inline void render_text_center(int x, int y, const char* text, uint32_t color)
{
	int len = strlen(text);
	v2_t pos = (v2_t) { x - (len*4), y-4 };
	render_text(pos.x, pos.y, text, color);
}

static inline void render_text_center_overlay(int x, int y, const char* text, uint32_t back_color, uint32_t front_color)
{
	int len = strlen(text);
	v2_t pos = (v2_t) { x - (len*4), y+4 };
	render_text_overlay(pos.x, pos.y, text, back_color, front_color);
}

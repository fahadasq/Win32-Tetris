#ifndef COLORS_H

#define COLORS_H

#include <stdint.h>

#define WHITE 0xFFFFFFFF

#define BRIGHT_ORANGE 0x00FF7930
#define MID_ORANGE 0x00E35100
#define DARK_ORANGE 0x00A23000

#define BRIGHT_CYAN 0x00A2FFF3
#define MID_CYAN 0x0061D3E3
#define DARK_CYAN 0x004192C3

#define BRIGHT_YELLOW 0x00FFF392
#define MID_YELLOW 0x00EBD320
#define DARK_YELLOW 0x008A8A00

#define BRIGHT_GREEN 0x00A2F3A2
#define MID_GREEN 0x0071F341
#define DARK_GREEN 0x0049AA10

#define BRIGHT_BLUE 0x0061A2FF
#define MID_BLUE 0x004161FB
#define DARK_BLUE 0x002000B2

#define BRIGHT_PURPLE 0x00A271FF
#define MID_PURPLE 0x009241F3
#define DARK_PURPLE 0x006110A2

#define BRIGHT_RED 0x00FF596B
#define MID_RED 0x00E12B3F
#define DARK_RED 0x00B21021

#define BRIGHT_BORDER 0x008485BF
#define MID_BORDER 0x005E5F91
#define DARK_BORDER 0x00535280 

typedef uint8_t tile;

#define TILE_EMPTY 0x0
#define TILE_ORANGE 0x1
#define TILE_CYAN 0x2
#define TILE_YELLOW 0x3
#define TILE_GREEN 0x4
#define TILE_BLUE 0x5
#define TILE_PURPLE 0x6
#define TILE_RED 0x7
#define TILE_BORDER 0x8

static inline uint32_t bright_tile_color(tile color)
{
	switch (color)
	{
	case TILE_ORANGE:
		return BRIGHT_ORANGE;
	case TILE_CYAN:
		return BRIGHT_CYAN;
	case TILE_YELLOW:
		return BRIGHT_YELLOW;
	case TILE_GREEN:
		return BRIGHT_GREEN;
	case TILE_BLUE:
		return BRIGHT_BLUE;
	case TILE_PURPLE:
		return BRIGHT_PURPLE;
	case TILE_RED:
		return BRIGHT_RED;
	case TILE_BORDER:
		return BRIGHT_BORDER;

	default:
		return TILE_EMPTY;
	}
}

static inline uint32_t mid_tile_color(tile color)
{
	switch (color)
	{
	case TILE_ORANGE:
		return MID_ORANGE;
	case TILE_CYAN:
		return MID_CYAN;
	case TILE_YELLOW:
		return MID_YELLOW;
	case TILE_GREEN:
		return MID_GREEN;
	case TILE_BLUE:
		return MID_BLUE;
	case TILE_PURPLE:
		return MID_PURPLE;
	case TILE_RED:
		return MID_RED;
	case TILE_BORDER:
		return MID_BORDER;

	default:
		return TILE_EMPTY;
	}
}

static inline uint32_t dark_tile_color(tile color)
{
	switch (color)
	{
	case TILE_ORANGE:
		return DARK_ORANGE;
	case TILE_CYAN:
		return DARK_CYAN;
	case TILE_YELLOW:
		return DARK_YELLOW;
	case TILE_GREEN:
		return DARK_GREEN;
	case TILE_BLUE:
		return DARK_BLUE;
	case TILE_PURPLE:
		return DARK_PURPLE;
	case TILE_RED:
		return DARK_RED;
	case TILE_BORDER:
		return DARK_BORDER;

	default:
		return TILE_EMPTY;
	}
}

#endif

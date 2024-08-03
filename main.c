#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "colors.h"
#include "tetromino.h"

#include "renderer.c"
#include "board.c"

typedef enum game_state
{
	game_state_menu,
	game_state_gameplay,
	game_state_loss
} game_state_t;


typedef struct input
{
	int enter;

	int right;
	int left;
	int down;

	int a;
	int d;

} input_t;

typedef enum line_clear_type
{
	line_clear_single = 0,
	line_clear_double = 1,
	line_clear_triple = 2,
	line_clear_tetris = 3
} line_clear_type_t;

typedef struct line_clear_message
{
	line_clear_type_t type;
	double time;
	uint8_t level;
} line_clear_message_t;

typedef struct state
{
	board_t board;
	tetromino_t current;
	tetromino_t next, second, third;
	game_state_t game_state;

	// the "tick" is the point at which the tetromino drops down 1 cell
	double time, tick_counter, tick_time_limit;

	line_clear_message_t latest_line_clear;

	uint8_t level;
	uint16_t lines;
	uint32_t score;

	uint32_t high_score;
} state_t;

static state_t state;
static input_t input;

#include "tetris.c"
#include "window.c"

int WINAPI WinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR command_line,
	int cmd_show)
{
	HWND window_handle = win32_create_window(instance);

	if (!window_handle) return 0;

	win32_init_dib_section();

	ShowWindow(window_handle, cmd_show);
	MSG msg = { 0 };

	double last_time = 0;

	state.current = spawn_tetrominos[0];
	state.next = spawn_tetrominos[1];
	state.second = spawn_tetrominos[2];
	state.third = spawn_tetrominos[3];
	state.tick_time_limit = 0.7987;
	state.game_state = game_state_menu;
	state.level = 1;

	while (msg.message != WM_QUIT)
	{
		last_time = state.time;
		state.time = win32_current_time_in_seconds();
		double delta_time = state.time - last_time;
		state.tick_counter += delta_time;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			switch (state.game_state)
			{
				case game_state_menu:
				{
					clear_screen();
					render_menu();

					HDC device_context = GetDC(window_handle);
					win32_update_window(device_context);
					ReleaseDC(window_handle, device_context);

					if (input.enter) state.game_state = game_state_gameplay;

					break;
				}
				case game_state_gameplay:
				{
					handle_tetromino_movement();

					if (state.tick_counter >= state.tick_time_limit)
					{
						step();
						state.tick_counter = 0.0;
					}

					clear_screen();

					draw_board_border();
					draw_board(state.board);

					draw_tetromino_on_board(state.current);

					render_gameplay_stats();
					render_preview_tetromino();

					HDC device_context = GetDC(window_handle);
					win32_update_window(device_context);
					ReleaseDC(window_handle, device_context);

					clear_input();
					break;
				}

				case game_state_loss:
				{
					clear_screen();
					render_loss_board();

					if (input.enter)
					{
						reset_game();
						state.game_state = game_state_gameplay;
					}

					HDC device_context = GetDC(window_handle);
					win32_update_window(device_context);
					ReleaseDC(window_handle, device_context);

					break;
				}

			}
		}
	}
	
	
	return 0;
}

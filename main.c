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

typedef enum line_clear
{
	line_clear_single = 0,
	line_clear_double = 1,
	line_clear_triple = 2,
	line_clear_tetris = 3
} line_clear_t;

typedef struct achievement_message
{
	line_clear_t type;
	double time;
	uint8_t level;
} achievement_message_t;

typedef struct state
{
	board_t board;
	tetromino_t current;
	tetromino_t next;
	game_state_t game_state;
	double time, tick_counter, tick_time_limit;

	achievement_message_t latest_achievement;

	uint8_t level;
	uint16_t lines;
	uint32_t score;

	uint32_t high_score;
	

} state_t;

static state_t state;
static input_t input;

#include "tetris.c"
#include "window.c"

int WINAPI wWinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPWSTR command_line,
	int cmd_show)
{
	HWND window_handle = win32_create_window(instance);

	if (!window_handle) return 0;

	win32_init_dib_section();

	ShowWindow(window_handle, cmd_show);
	MSG msg = { 0 };

	double last_time = 0;

	state.current = spawn_tetrominos[6];
	state.next = spawn_tetrominos[5];
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
					render_menu_logo();

					uint32_t back_color = (int)(floor(state.time*6)) % 2 ? DARK_YELLOW : DARK_YELLOW;
					uint32_t front_color = (int)(floor(state.time*6)) % 2 ? BRIGHT_YELLOW : WHITE;

					render_text_center_overlay(200, 200, "PRESS ENTER TO PLAY", back_color, front_color);

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
					draw_tetromino_on_board(state.current);

					render_preview_tetromino();

					draw_board(state.board);

					render_gameplay_stats();

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

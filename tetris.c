#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define COLLISION_RIGHT 0x1
#define COLLISION_LEFT 0x1 << 1
#define COLLISION_BOTTOM 0x1 << 2
#define COLLISION_CEILING 0x1 << 3

static inline void draw_board_border();
static inline void reset_game();

static inline void render_menu_logo()
{
}

static inline void handle_tetromino_movement();

static inline void render_preview_tetromino()
{
	render_text_overlay(32+1, 27+1, "NEXT", DARK_BORDER, MID_BORDER);
	render_text(32, 27, "NEXT", WHITE);
	draw_tetromino(-7, 3, state.next);
}

const char line_clear_string[4][9] =
{
	"SINGLE!",
	"DOUBLE!",
	"TRIPLE!",
	"TETRIS!!"
};

static inline uint16_t line_clear_points(line_clear_t type)
{
	switch (type)
	{
		case line_clear_single:
			return 40;
		case line_clear_double:
			return 100;
		case line_clear_triple:
			return 300;
		case line_clear_tetris:
			return 1200;
	}
}

static inline void render_achievement_message()
{
	render_text_overlay(25, 150, line_clear_string[state.latest_achievement.type], DARK_RED, WHITE);

	char buf[11];
	sprintf(buf, "%u * %u", line_clear_points(state.latest_achievement.type), state.level);
	render_text_overlay(25, 162, buf, DARK_RED, WHITE);

	sprintf(buf, "+ %u", line_clear_points(state.latest_achievement.type) * state.level);
	render_text_overlay(25, 174, buf, DARK_RED, WHITE);
}

static inline void render_gameplay_stats()
{
	const int pos_x = 325;
	const int pos_y = 50;

	render_text(pos_x+2, pos_y+2, "LINES", DARK_RED);
	render_text(pos_x+1, pos_y+1, "LINES", MID_RED);
	render_text(pos_x, pos_y, "LINES", WHITE);

	char lines_buf[5] = {0};
	sprintf(lines_buf, "%u", state.lines);

	render_text(pos_x+2, pos_y+2+12, lines_buf, DARK_RED);
	render_text(pos_x+1, pos_y+1+12, lines_buf, MID_RED);
	render_text(pos_x, pos_y+12, lines_buf, WHITE);

	char score_buf[11] = {0};
	sprintf(score_buf, "%u", state.score);

	render_text(pos_x+2, pos_y+2+32, "SCORE", DARK_BLUE);
	render_text(pos_x+1, pos_y+1+32, "SCORE", MID_BLUE);
	render_text(pos_x, pos_y+32, "SCORE", WHITE);

	render_text(pos_x+2, pos_y+2+12+32, score_buf, DARK_BLUE);
	render_text(pos_x+1, pos_y+1+12+32, score_buf, MID_BLUE);
	render_text(pos_x, pos_y+12+32, score_buf, WHITE);

	char level_buf[3] = {0};
	sprintf(level_buf, "%u", state.level-1);

	render_text(pos_x+2, pos_y+2+64, "LEVEL", DARK_YELLOW);
	render_text(pos_x+1, pos_y+1+64, "LEVEL", MID_YELLOW);
	render_text(pos_x, pos_y+64, "LEVEL", WHITE);

	render_text(pos_x+2, pos_y+2+12+64, level_buf, DARK_YELLOW);
	render_text(pos_x+1, pos_y+1+12+64, level_buf, MID_YELLOW);
	render_text(pos_x, pos_y+12+64, level_buf, WHITE);

	if (state.latest_achievement.time + 5.0 >= state.time)
	{
		render_achievement_message();
	}
}

static inline void render_loss_board()
{
	draw_board_border();
	draw_board(state.board);
	draw_quad(125-2, 90-2, 150+4, 120+4, 0x10);
	draw_quad(125, 90, 150, 120, 0x40);

	render_text_center_overlay(200, 100, "GAME OVER", DARK_RED, WHITE);
	
	char score_buf[11];
	sprintf(score_buf, "%u", state.score);

	render_text_center_overlay(200, 125, "SCORE", DARK_YELLOW, WHITE);
	render_text_center_overlay(200, 137, score_buf, DARK_YELLOW, WHITE);

	char high_score_buf[11];
	sprintf(high_score_buf, "%u", state.high_score);

	render_text_center_overlay(200, 162, "HIGHSCORE", DARK_BLUE, WHITE);
	render_text_center_overlay(200, 174, high_score_buf, DARK_BLUE, WHITE);

}

static inline int check_tetromino_collisions(tetromino_t ttr, board_t board)
{
	int result = 0;

	for (int yy = 0; yy < 4; yy++)
	{
		for (int xx = 0; xx < 4; xx++)
		{
			int x = max(min(ttr.position.x+xx, 9), 0);
			int y = max(min(ttr.position.y+yy, 19), 0);

			if ( (ttr.shape  >> (4*(3-yy)) ) & (1 << (3-xx)))
			{
				if (y == 0)
				{
					result |= COLLISION_CEILING;
				}

				if (y == 19)
				{
					result |= COLLISION_BOTTOM;
				}

				else if (board.tiles[y+1][x])
				{
					result |= COLLISION_BOTTOM;
				}

				if (x == 9)
				{
					result |= COLLISION_RIGHT;
				}
				else if (board.tiles[y][x+1])
				{
					result |= COLLISION_RIGHT;
				}

				if (x == 0)
				{
					result |= COLLISION_LEFT;
				}
				else if (board.tiles[y][x-1])
				{
					result |= COLLISION_LEFT;
				}
			}
		}
	}

	return result;
}

static inline uint8_t get_right_rotation()
{
	if (state.current.rotation == ROTATION_LEFT) return ROTATION_SPAWN;
	else return (state.current.rotation + 1);
}

static inline uint8_t get_left_rotation()
{
	if (state.current.rotation == ROTATION_SPAWN) return ROTATION_LEFT;
	else return (state.current.rotation - 1);
}


static inline int check_tetromino_rotation_possible(int right)
{
	uint8_t new_rotation = (right) ? get_right_rotation() : get_left_rotation();
	uint16_t new_shape = state.current.shape_index[new_rotation];

	for (int yy = 0; yy < 4; yy++)
	{
		for (int xx = 0; xx < 4; xx++)
		{
			int x = state.current.position.x+xx;
			int y = state.current.position.y+yy;

			if ((new_shape  >> (4*(3-yy)) ) & (1 << (3-xx)))
			{
				if (x > 9 || x < 0) return 0;
				if (y > 19 || y < 0) return 0;

				if (state.board.tiles[y][x])
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

static inline int check_tetromino_right_rotation_possible() { return check_tetromino_rotation_possible(1); }
static inline int check_tetromino_left_rotation_possible() { return check_tetromino_rotation_possible(0); }

static inline void rotate_ttr_right()
{
	state.current.rotation = get_right_rotation();
	state.current.shape = state.current.shape_index[state.current.rotation];
}

static inline void rotate_ttr_left()
{
	state.current.rotation = get_left_rotation();
	state.current.shape = state.current.shape_index[state.current.rotation];
}

static inline void spawn()
{
	state.current = state.next;

	int num = rand() % 7;

	state.next = spawn_tetrominos[num];
}

static inline void reset_game()
{
	clear_board(&state.board);
	state.lines = 0;
	state.score = 0;
	state.level = 1;
	state.tick_counter = 0;
	spawn();
}

static inline void handle_board()
{
	uint8_t complete_lines_counter = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		int row_is_complete = 1;

		for(int x = 0; x < BOARD_WIDTH; x++)
		{
			if (!state.board.tiles[y][x])
			{
				row_is_complete = 0;
				break;
			}
		}

		if (row_is_complete)
		{
			complete_lines_counter++;
			state.lines++;

			for (int r = y; r > 0; r--)
			{
				memcpy(&state.board.tiles[r], &state.board.tiles[r-1], sizeof(tile) * 10);
			}
		}
	}

	switch (complete_lines_counter)
	{
		default: break;
		case 1:
		{
			state.score += 40 * state.level;
			state.latest_achievement.time = state.time;
			state.latest_achievement.type = line_clear_single;
			state.level = state.level;
			break;
		}
		case 2:
		{
			state.score += 100 * state.level;
			state.latest_achievement.time = state.time;
			state.latest_achievement.type = line_clear_double;
			state.level = state.level;
			break;
		}
		case 3:
		{
			state.score += 300 * state.level;
			state.latest_achievement.time = state.time;
			state.latest_achievement.type = line_clear_triple;
			state.level = state.level;
			break;
		}
		case 4:
		{
			state.score += 1200 * state.level;
			state.latest_achievement.time = state.time;
			state.latest_achievement.type = line_clear_tetris;
			state.level = state.level;
			break;
		}
	}

	if (state.lines >= 60) { state.level = 7; state.tick_time_limit = 0.3827; }
	else if (state.lines >= 50) { state.level = 6; state.tick_time_limit = 0.4659; }
	else if (state.lines >= 40) { state.level = 5; state.tick_time_limit = 0.5491; }
	else if (state.lines >= 30) { state.level = 4; state.tick_time_limit = 0.6323; }
	else if (state.lines >= 20) { state.level = 3; state.tick_time_limit = 0.7155; }
	else if (state.lines >= 10) { state.level = 2; state.tick_time_limit = 0.7987; }
}

static inline void step()
{
	int collision = check_tetromino_collisions(state.current, state.board);
	if (collision & COLLISION_BOTTOM)
	{
		if (collision & COLLISION_CEILING)
		{
			state.game_state = game_state_loss;
			if (state.high_score < state.score)
			{
				state.high_score = state.score;
			}
			return;
		}

		set_tetromino_on_board(&state.board, state.current);
		handle_board();
		spawn();
	}
	else
	{
		state.current.position.y++;
	}
}

static inline void handle_tetromino_movement()
{
	int collision = check_tetromino_collisions(state.current, state.board);

	if (input.down && !(collision & COLLISION_BOTTOM))
	{
		state.tick_counter = 0.0;
		state.current.position.y++;
	}

	if (input.right && !(collision & COLLISION_RIGHT))
	{
		state.current.position.x++;
	}

	if (input.left && !(collision & COLLISION_LEFT))
	{
		state.current.position.x--;
	}

	if (input.a)
	{
		if (check_tetromino_left_rotation_possible())
		rotate_ttr_left();
	}

	if (input.d)
	{
		if (check_tetromino_right_rotation_possible())
		rotate_ttr_right();
	}

}

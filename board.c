#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_LEFT ((BITMAP_WIDTH - (TILE_SIZE*BOARD_WIDTH) ) / 2)
#define BOARD_TOP 0

typedef struct board
{
	tile tiles[20][10];
} board_t;

static inline void clear_board(board_t* board)
{
	memset(board, 0, sizeof(board_t));
}

static inline void draw_tile_on_board(int x, int y, tile color)
{
	draw_tile(BOARD_LEFT + (x * TILE_SIZE), BOARD_TOP + (y * TILE_SIZE), color);
}

static inline void draw_board_border()
{
	for (int y = 0; y < 20; y++)
	{
		draw_tile_on_board(-1, y, TILE_BORDER);
		draw_tile_on_board(10, y, TILE_BORDER);
	}
	for (int x = -1; x < 11; x++)
	{
		draw_tile_on_board(x, 20, TILE_BORDER);
	}
}

static inline void draw_board(board_t board)
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			tile board_tile = board.tiles[y][x];

			if (board_tile)
			{
				draw_tile_on_board(x, y, board_tile);
			}
		}
	}
}

static inline void draw_tetromino_on_board(tetromino_t ttr)
{
	for (int yy = 0; yy < 4; yy++)
	{
		for (int xx = 0; xx < 4; xx++)
		{
			int x = max(min(ttr.position.x+xx, 9), 0);
			int y = max(min(ttr.position.y+yy, 19), 0);

			if ( (ttr.shape  >> (4*(3-yy)) ) & (1 << (3-xx)))
			{
				draw_tile_on_board(x, y, ttr.color);
			}
		}
	}
}

static inline void draw_tetromino(int x, int y, tetromino_t ttr)
{
	for (int yy = 0; yy < 4; yy++)
	{
		for (int xx = 0; xx < 4; xx++)
		{
			if ( (ttr.shape  >> (4*(3-yy)) ) & (1 << (3-xx)))
			{
				draw_tile_on_board(x+xx, y+yy, ttr.color);
			}
		}
	}
}

static inline void set_tetromino_on_board(board_t* board, tetromino_t ttr)
{
	for (int yy = 0; yy < 4; yy++)
	{
		for (int xx = 0; xx < 4; xx++)
		{
			int x = max(min(ttr.position.x+xx, 9), 0);
			int y = max(min(ttr.position.y+yy, 19), 0);

			if ( (ttr.shape  >> (4*(3-yy)) ) & (1 << (3-xx)))
			{
				board->tiles[y][x] = ttr.color;
			}
		}
	}
}

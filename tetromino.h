#ifndef TETROMINO_H
#define TETROMINO_H

typedef struct v2
{
	int16_t x;
	int16_t y;
} v2_t;

#define ROTATION_SPAWN 0
#define ROTATION_RIGHT 1
#define ROTATION_180   2
#define ROTATION_LEFT  3

typedef struct tetromino
{
	v2_t position;
	uint8_t rotation;
	const uint16_t* shape_index;
	uint16_t shape;
	uint8_t color;
} tetromino_t;

static const uint16_t tetromino_I[4] = 
{
	/*
	  SPAWN:
	  0 0 0 0
	  1 1 1 1
	  0 0 0 0
	  0 0 0 0
	 */
	0b0000111100000000,

	/*
	  R:
	  0 0 1 0
	  0 0 1 0
	  0 0 1 0
	  0 0 1 0
	 */
	0b0010001000100010,
		
	/*
	  180:
	  0 0 0 0
	  0 0 0 0
	  1 1 1 1
	  0 0 0 0
	 */
	0b0000000011110000,

	/*
	  L:
	  0 1 0 0
	  0 1 0 0
	  0 1 0 0
	  0 1 0 0
	 */
	0b0100010001000100
};

static const uint16_t tetromino_Z[4] =
{
	/*
	  SPAWN:
	  1100
	  0110
	  0000
	  0000
	*/
	0b1100011000000000,

	/*
	  R:
	  0010
	  0110
	  0100
	  0000
	*/
	0b0010011001000000,

	/*
	  180:
	  0000
	  1100
	  0110
	  0000
	 */
	0b0000110001100000,

	/*
	  L:
	  0100
	  1100
	  1000
	  0000
	 */
	0b0100110010000000
};

static const uint16_t tetromino_L[4] =
{
	/*
	  SPAWN:
	  0010
	  1110
	  0000
	  0000
	 */
	0b0010111000000000,

	/*
	  R:
	  0100
	  0100
	  0110
	  0000
	 */
	0b0100010001100000,

	/*
	  180:
	  0000
	  1110
	  1000
	  0000
	 */
	0b0000111010000000,

	/*
	  L:
	  1100
	  0100
	  0100
	  0000
	 */
	0b1100010001000000
};

static const uint16_t tetromino_S[4] =
{
	/*
	  SPAWN:
	  0110
	  1100
	  0000
	  0000
	 */
	0b0110110000000000,
	
	/*
	  R:
	  0100
	  0110
	  0010
	  0000
	 */
	0b0100011000100000,

	/*
	  180:
	  0000
	  0110
	  1100
	  0000
	 */
	0b0000011011000000,

	/*
	  L:
	  1000
	  1100
	  0100
	  0000
	 */
	0b1000110001000000
};

static const uint16_t tetromino_J[4] =
{
	/*
	  SPAWN:
	  1000
	  1110
	  0000
	  0000
	 */
	0b1000111000000000,

	/*
	  R:
	  0110
	  0100
	  0100
	  0000
	*/
	0b0110010001000000,

	/*
	  180:
	  0000
	  1110
	  0010
	  0000
	*/
	0b0000111000100000,

	/*
	  L:
	  0100
	  0100
	  1100
	  0000
	*/
	0b0100010011000000
};

static const uint16_t tetromino_T[4] =
{
	/*
	  SPAWN:
	  0100
	  1110
	  0000
	  0000
	*/
	0b0100111000000000,

	/*
	  R:
	  0100
	  0110
	  0100
	  0000
	*/
	0b0100011001000000,

	/*
	  180:
	  0000
	  1110
	  0100
	  0000
	*/
	0b0000111001000000,

	/*
	  L:
	  0100
	  1100
	  0100
	  0000
	*/
	0b0100110001000000
};

static const uint16_t tetromino_O[4] =
{
	/* no rotations, all the same
	   0110
	   0110
	   0000
	   0000
	*/
	0b0110011000000000,
	0b0110011000000000,
	0b0110011000000000,
	0b0110011000000000
};

#define SPAWN_POSITION (v2_t) { 3, 0 }

static const tetromino_t spawn_tetrominos[7] =
{
	(tetromino_t) {
		(v2_t) { 3, 0 },
		ROTATION_SPAWN,
		tetromino_I,
		tetromino_I[ROTATION_SPAWN],
		TILE_CYAN
	},

	(tetromino_t) {
		SPAWN_POSITION,
		ROTATION_SPAWN,
		tetromino_J,
		tetromino_J[ROTATION_SPAWN],
		TILE_BLUE
	},

	(tetromino_t) {
		SPAWN_POSITION,
		ROTATION_SPAWN,
		tetromino_L,
		tetromino_L[ROTATION_SPAWN],
		TILE_ORANGE
	},

	(tetromino_t) {
		SPAWN_POSITION,
		ROTATION_SPAWN,
		tetromino_O,
		tetromino_O[ROTATION_SPAWN],
		TILE_YELLOW
	},

	(tetromino_t) {
		SPAWN_POSITION,
		ROTATION_SPAWN,
		tetromino_S,
		tetromino_S[ROTATION_SPAWN],
		TILE_GREEN
	},

	(tetromino_t) {
		SPAWN_POSITION,
		ROTATION_SPAWN,
		tetromino_Z,
		tetromino_Z[ROTATION_SPAWN],
		TILE_RED
	},

	(tetromino_t) {
		SPAWN_POSITION,
		ROTATION_SPAWN,
		tetromino_T,
		tetromino_T[ROTATION_SPAWN],
		TILE_PURPLE
	}
};

#endif

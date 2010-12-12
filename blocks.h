/**
 * blocks.h
 *
 * Blocks game implementation
 *
 * @author Timothy Cheeseman
 */

#ifndef _BLOCKS_H
#define _BLOCKS_H

#include <stdbool.h>
#include <stdint.h>

/**
 * The height of the buffer above the visible game area where new pieces are inserted
 */
static const int BLOCKS_BUFFER_HEIGHT = 4;

/**
 * RGB color
 */
typedef struct Color {

	uint8_t r;
	uint8_t g;
	uint8_t b;

} Color;

/**
 * Tetromino representation
 */
typedef struct Tetromino
{
	Color color;
	int width;
	int height;
	uint8_t **mask;
	int position[2];
	
} Tetromino;

/**
 * Tetromino type enum
 */
enum TetrominoType {
	
	TETROMINO_I = 0,
	TETROMINO_J = 1,
	TETROMINO_L = 2,
	TETROMINO_O = 3,
	TETROMINO_S = 4,
	TETROMINO_Z = 5,
	TETROMINO_T = 6
};

/**
 * Blocks game representation
 */
typedef struct BlocksGame {

	int width;
	int height;
	
	Tetromino *current_piece;
	Tetromino *next_piece;
	
	uint8_t **mask;
	
	long score;
	int score_multiplier;
	bool game_over;
	
} BlocksGame;

/**
 * Direction enum
 */
typedef enum Direction {
	
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_DOWN

} Direction;

/**
 * Create a new blocks game
 */
BlocksGame *blocksNewGame(int width, int height);

/**
 * Attempt to move the current piece in a blocks game
 */
void blocksMovePiece(BlocksGame *game, Direction direction);

/**
 * Attempt to rotate the current piece in a blocks game
 */
void blocksRotatePiece(BlocksGame *game);

/**
 * Attempt to drop the current piece in a blocks game
 */
void blocksDropPiece(BlocksGame *game);

/**
 * Free the memory used by a blocks game
 */
void blocksFreeGame(BlocksGame *game);

#endif /* _BLOCKS_H */

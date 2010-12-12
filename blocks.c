/**
 * blocks.c
 *
 * Blocks game implementation
 *
 * @author Timothy Cheeseman
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "blocks.h"

static const Color TetrominoColors[] = {
	{255, 0, 0}, // red
	{0, 255, 0}, // green
	{0, 0, 255}, //  blue
	{255, 255, 0}, // yellow
	{255, 165, 0}, // orange
	{255, 0, 255}, // magenta
	{0, 255, 255} // cyan
};

/**
 * Print an error to stderr and exit with EXIT_FAILURE
 */
static void blocksError(const char* message);

/**
 * Generate a random tetromino
 */
static Tetromino *blocksRandomTetromino(int gameWidth);

/**
 * Merge the old current piece into the game mask and cycle the new piece
 */
static void blocksNextPiece(BlocksGame *game);

/**
 * Check if there is a collision between the current piece and the game mask
 */
static bool blocksCollision(BlocksGame *game);

/**
 * Update game state (score, game over status, and cleared rows)
 */
static void blocksUpdateState(BlocksGame *game);

/**
 * Allocate a 2D byte mask
 */
static uint8_t **blocksAllocMask(int width, int height);

/**
 * Free the memory used by an allocated byte mask
 */
static void blocksFreeMask(uint8_t **mask, int height);

/**
 * Free the memory used by a tetromino
 */
static void blocksFreeTetromino(Tetromino *tetromino);

static void blocksError(const char* message)
{
	fprintf(stderr, "BLOCKS3D: %s\n", message);
	exit(EXIT_FAILURE);	
}

BlocksGame *blocksNewGame(int width, int height)
{
	BlocksGame *game = malloc(sizeof(BlocksGame));
	
	if(!game)
		blocksError("Error allocating memory for a new Blocks3D game.");
	
	game->width = width;
	game->height = height + BLOCKS_BUFFER_HEIGHT;
	
	game->mask = blocksAllocMask(game->width, game->height);
	
	srand(time(NULL));
	game->current_piece = blocksRandomTetromino(game->width);
	game->next_piece = blocksRandomTetromino(game->width);
	
	game->score = 0;
	game->score_multiplier = 1;
	game->game_over = false;
	
	return game;
}

static Tetromino *blocksRandomTetromino(int gameWidth)
{
	Tetromino *next_piece = malloc(sizeof(Tetromino));
	
	if(!next_piece)
		blocksError("Error allocating memory for a new tetromino.");
	
	int piece = rand() % 7;
	switch(piece)
	{
		case TETROMINO_I:
			next_piece->width = 1;
			next_piece->height = 4;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][0] = 1;
			next_piece->mask[1][0] = 1;
			next_piece->mask[2][0] = 1;
			next_piece->mask[3][0] = 1;
			break;
		case TETROMINO_J:
			next_piece->width = 2;
			next_piece->height = 3;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][1] = 1;
			next_piece->mask[1][1] = 1;
			next_piece->mask[2][1] = 1;
			next_piece->mask[2][0] = 1;
			break;
		case TETROMINO_L:
			next_piece->width = 2;
			next_piece->height = 3;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][0] = 1;
			next_piece->mask[1][0] = 1;
			next_piece->mask[2][0] = 1;
			next_piece->mask[2][1] = 1;
			break;
		case TETROMINO_O:
			next_piece->width = 2;
			next_piece->height = 2;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][0] = 1;
			next_piece->mask[0][1] = 1;
			next_piece->mask[1][0] = 1;
			next_piece->mask[1][1] = 1;
			break;
		case TETROMINO_S:
			next_piece->width = 3;
			next_piece->height = 2;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][1] = 1;
			next_piece->mask[0][2] = 1;
			next_piece->mask[1][0] = 1;
			next_piece->mask[1][1] = 1;
			break;
		case TETROMINO_Z:
			next_piece->width = 3;
			next_piece->height = 2;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][0] = 1;
			next_piece->mask[0][1] = 1;
			next_piece->mask[1][1] = 1;
			next_piece->mask[1][2] = 1;
			break;
		case TETROMINO_T:
			next_piece->width = 3;
			next_piece->height = 2;
			next_piece->mask = blocksAllocMask(next_piece->width, next_piece->height);
			next_piece->mask[0][1] = 1;
			next_piece->mask[1][0] = 1;
			next_piece->mask[1][1] = 1;
			next_piece->mask[1][2] = 1;
			break;
	}
	
	next_piece->position[0] = gameWidth / 2 - 2;
	next_piece->position[1] = BLOCKS_BUFFER_HEIGHT - next_piece->height;
	
	next_piece->color = TetrominoColors[piece];
	
	return next_piece;
}

void blocksMovePiece(BlocksGame *game, Direction direction)
{
	if(game->game_over)
		return;

	switch (direction)
	{
		case DIRECTION_LEFT:
			game->current_piece->position[0]--;
		
			if(blocksCollision(game))
				game->current_piece->position[0]++;
			break;
		case DIRECTION_RIGHT:
			game->current_piece->position[0]++;
			
			if(blocksCollision(game))
				game->current_piece->position[0]--;
			break;
		case DIRECTION_DOWN:
			game->current_piece->position[1]++;
			
			if(!blocksCollision(game))
				return;
			
			game->current_piece->position[1]--;
			
			blocksNextPiece(game);
			break;
	}
}

void blocksDropPiece(BlocksGame *game)
{
	if(game->game_over)
		return;
	
	while(!blocksCollision(game))
		game->current_piece->position[1]++;
	
	game->current_piece->position[1]--;

	blocksNextPiece(game);
}

static void blocksNextPiece(BlocksGame *game)
{
	if(game->game_over)
		return;
	
	int i, j;

	// merge old piece into game mask
	
	for (i = 0; i < game->current_piece->height; i++)
	{
		int y = game->current_piece->position[1] + i;
		
		for (j = 0; j < game->current_piece->width; j++)
		{
			int x = game->current_piece->position[0] + j;
			
			if(game->current_piece->mask[i][j])
				game->mask[y][x] = 1;
		}
	}
	
	// cycle pieces
	
	blocksFreeTetromino(game->current_piece);
	game->current_piece = game->next_piece;
	game->next_piece = blocksRandomTetromino(game->width);
	
	// update game state after each dropped piece
	
	blocksUpdateState(game);
}

void blocksRotatePiece(BlocksGame *game)
{
	if(game->game_over)
		return;
	
	int i, j;
	uint8_t **old_mask;
	
	int new_width = game->current_piece->height;
	int new_height = game->current_piece->width;
	uint8_t **new_mask = blocksAllocMask(new_width, new_height);
	
	for(i = 0; i < new_height; i++)
		for(j = 0; j < new_width; j++)
			new_mask[i][j] = game->current_piece->mask[game->current_piece->height - j - 1][i];
	
	old_mask = game->current_piece->mask;
	game->current_piece->mask = new_mask;
	game->current_piece->height = new_height;
	game->current_piece->width = new_width;
	
	// if rotation causes a collision, undo it
	
	if(blocksCollision(game))
	{
		game->current_piece->mask = old_mask;
		game->current_piece->height = new_width;
		game->current_piece->width = new_height;
		blocksFreeMask(new_mask, new_height);
		
		return;
	}

	blocksFreeMask(old_mask, new_width);
}

static bool blocksCollision(BlocksGame *game)
{
	int i, j;
	
	// check for out of bounds
	
	if(game->current_piece->position[0] < 0)
		return true;
	
	if(game->current_piece->position[0] + game->current_piece->width > game->width)
		return true;
	
	if(game->current_piece->position[1] + game->current_piece->height > game->height)
		return true;
	
	// check for collisions
	
	for (i = 0; i < game->current_piece->height; i++)
	{
		int y = game->current_piece->position[1] + i;
		
		for (j = 0; j < game->current_piece->width; j++)
		{
			int x = game->current_piece->position[0] + j;
			
			if(game->current_piece->mask[i][j] && game->mask[y][x])
				return true;
		}
	}
	
	return false;
}

static void blocksUpdateState(BlocksGame *game)
{
	int i, j, k;
	
	// update score for landing piece
	
	game->score += game->score_multiplier * 100;
	
	// check for full rows
	
	for (i = 0; i < game->height; i++)
	{
		// check for full row
		
		bool full_row = true;
		
		for (j = 0; j < game->width; j++)
			if(!game->mask[i][j])
				full_row = false;
		
		if(full_row)
		{
			// update score for full row
			
			game->score += game->score_multiplier * 1000;
			
			// clear row
			
			for (j = 0; j < game->width; j++)
				game->mask[i][j] = 0;
			
			// move all rows above down
			
			for(k = i - 1; k >= 0; k--)
			{
				for (j = 0; j < game->width; j++)
				{
					if(game->mask[k][j])
					{
						game->mask[k + 1][j] = 1;
						game->mask[k][j] = 0;
					}
				}
			}
		}
	}
	
	// check for game over
	
	for (i = 0; i < BLOCKS_BUFFER_HEIGHT; i++)
		for (j = 0; j < game->width; j++)
			if(game->mask[i][j])
				game->game_over = true;
}

static uint8_t **blocksAllocMask(int width, int height)
{
	int i, j;
	
	uint8_t **mask = malloc(height * sizeof(uint8_t *));
	const char *error_message = "Error allocating memory for a mask.";
	
	if(!mask)
		blocksError(error_message);
	
	for(i = 0; i < height; i++)
	{
		mask[i] = malloc(width * sizeof(uint8_t *));
		
		if(!mask[i])
			blocksError(error_message);
		
		for (j = 0; j < width; j++)
			mask[i][j] = 0;
	}
	
	return mask;
}

void blocksFreeGame(BlocksGame *game)
{
	blocksFreeTetromino(game->current_piece);
	blocksFreeTetromino(game->next_piece);
	blocksFreeMask(game->mask, game->height);
	
	free(game);
}

static void blocksFreeTetromino(Tetromino *tetromino)
{
	blocksFreeMask(tetromino->mask, tetromino->height);
	
	free(tetromino);
}

static void blocksFreeMask(uint8_t **mask, int height)
{
	int i;
	
	for(i = 0; i < height; i++)
		free(mask[i]);
	
	free(mask);
}

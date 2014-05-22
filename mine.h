#ifndef MINE_H
#define MINE_H
#include <stdbool.h>

#define IS_MINE(mine) ((mine).adj_mines == 9)

//! The mine type.
/**
 * Used for each square in the minesweeper grid.
 */
typedef struct
{
	int adj_mines; 
	///< Number of adjacent mines to this square. Is 9 if it is a mine,
					/// is -1 if this value hasn't been calculated yet.
	bool visible;
	bool flagged;
} Mine;

enum operation{
	UNDEFINED, ///< No operation should be taken
	REVEAL_MINE,
	TOGGLE_FLAG,
	QUIT
};

bool is_valid(int x, int y, int size);
Mine **create_board(int size, double mine_prob);
void reveal_board(int size, Mine** board);
void reveal_mines(int x, int y, int size, Mine** board);
void toggle_flag(Mine *mine);

#endif

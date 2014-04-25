#ifndef MINE_H
#define MINE_H
#include <stdbool.h>

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
} Mine;

bool is_valid(int x, int y, int size);
Mine **create_board(int size, double mine_prob);
void reveal_board(int size, Mine** board);

#endif

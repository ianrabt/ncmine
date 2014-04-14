#ifndef MINE_H
#define MINE_H
#include <stdbool.h>

typedef struct
{
	int adj_mines;
	bool visible;
} Mine;

bool is_valid(int x, int y, int size);
Mine **create_board(int size, double mine_prob);

#endif

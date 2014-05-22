#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "mine.h"

bool is_valid(int x, int y, int size)
{
	return x >= 0 && y >= 0 && x < size && y < size;
}

void toggle_flag(Mine *mine)
{
	assert(mine != NULL);
	mine->flagged = !mine->flagged;
}

bool get_prob_result(double prob)
{
    if(prob >= 1) {
        return true;
    } else if (prob <= 0) {
		return false;
	} else {
    	return rand() < prob * ((double) RAND_MAX + 1.0);
	}
}

void init_adj_mines(int size, Mine **board);
Mine **create_board(int size, double mine_prob)
{
	// allocating memory for the board.
	Mine **board;
	board = (Mine **) malloc(sizeof(Mine *) * size);
	if(board == NULL) {
		fprintf(stderr, "out of memory.\n");
		abort();
	}

	for(int i = 0; i < size; i++)
	{
		board[i] = (Mine *) malloc(sizeof(Mine)*size);
		if(board[i] == NULL) {
			fprintf(stderr, "out of memory.\n");
			abort();
		}
	}

	srand(time(NULL));

	// initializing board.
	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {

			if(get_prob_result(mine_prob)) {
				board[x][y].adj_mines = 9;
			} else {
				board[x][y].adj_mines = -1;
			}
			board[x][y].visible = false;
			board[x][y].flagged = false;
		}
	}

	init_adj_mines(size, board);
		
	return board;
}

/**
 * Returns a NULL terminated list of mines adj to the specified
 * coordinates
 */
void get_adj_mines(Mine *mines[9], int size, Mine **board, int x, int y)
{
    int i = 0;

    if(is_valid(x - 1, y - 1, size))
        mines[i++] = &board[x - 1][y - 1];
    if(is_valid(x    , y - 1, size))
        mines[i++] = &board[x    ][y - 1];
    if(is_valid(x + 1, y - 1, size))
        mines[i++] = &board[x + 1][y - 1];

    if(is_valid(x - 1, y    , size))
        mines[i++] = &board[x - 1][y    ];
    if(is_valid(x + 1, y    , size))
        mines[i++] = &board[x + 1][y    ];

    if(is_valid(x - 1, y + 1, size))
        mines[i++] = &board[x - 1][y + 1];
    if(is_valid(x    , y + 1, size))
        mines[i++] = &board[x    ][y + 1];
    if(is_valid(x + 1, y + 1, size))
        mines[i++] = &board[x + 1][y + 1];
    
    mines[i] = NULL;
}

void init_adj_mines(int size, Mine **board)
{
	assert(size > 0);

    for(int y = 0; y < size; y++) {
        for(int x = 0; x < size; x++) {
            if(board[x][y].adj_mines != -1) { // if the square is a mine,
                continue;            // or adj_mines is already calculated
            }                        // for some reason, continue
                                     // with the loop

            int *num_adj_mines = &(board[x][y].adj_mines); // pointer to save space.
            *num_adj_mines = 0; // "zero" adj_mines in order to begin counting mines.

            Mine *mines_array[9];
            get_adj_mines(mines_array, size, board, x, y);
			int i = 0;
			while(mines_array[i] != NULL) {
                *num_adj_mines += mines_array[i]->adj_mines == 9 ? 1: 0;
				i++;
            }
        }
    }
}

void reveal_board(int size, Mine** board)
{
	assert(size > 0);

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			board[x][y].visible = true;
		}
	}
}

void find_mine(int size, Mine** board, const Mine *query, int *x, int *y)
{
	assert(size > 0);
    assert(x != NULL);
    assert(y != NULL);
    assert(query != NULL);

    for(*y = 0; *y < size; (*y)++) {
        for(*x = 0; *x < size; (*x)++) {
            if(&board[*x][*y] == query) {
                return;
            }
        }
    }
    *x = -1; *y = -1;
}

void reveal_mines(int x, int y, int size, Mine** board)
{
	assert(size > 0);
	assert(x >= 0);
	assert(y >= 0);
	assert(x < size);
	assert(y < size);

    board[x][y].visible = true;
	if (board[x][y].adj_mines != 0)
		return;
    Mine *mines_array[9];
    Mine **mines = mines_array;
    get_adj_mines(mines_array, size, board, x, y);
    while(*mines) {
        if(!(**mines).visible) {
            (**mines).visible = true;
            if((**mines).adj_mines == 0) {
                int x2, y2;
                find_mine(size, board, *mines, &x2, &y2);
                reveal_mines(x2, y2, size, board);
            }
        }
        mines++;
    }
}

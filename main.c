#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "mine.h"

int main (int argc, char** argv)
{
	int size = 15;
	Mine **board = create_board(size, .15);

	startd();
	bool gameover = false;
	while (!gameover) {
		int x, y;
		printd(size, board);
		getin(&x, &y);
		board[x][y].visible = true;
		if (board[x][y].adj_mines == -1)
			gameover = true;
	}
	reveal_board(size, board);
	printd(size, board);
		
	exitd();
	free(board);
}

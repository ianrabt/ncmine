#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "mine.h"

int main (int argc, char** argv)
{
	int size = 10;
	Mine **board = create_board(size, .15);

	startd(size);
	bool gameover = false;
	while (!gameover) {
		int x, y;
		printd(size, board);
		getin(&x, &y);
		reveal_mines(x, y, size, board);
		if (is_mine(&board[x][y]))
			gameover = true;
	}
	reveal_board(size, board);
	printd(size, board);

	exitd();
	free(board);
}

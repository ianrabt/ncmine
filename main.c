#include <stdlib.h>
#include <ncurses.h>
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
		enum operation op;

		printd(size, board);
		op = getin(&x, &y);
		switch (op) {
		case TOGGLE_FLAG:
			toggle_flag(&board[x][y]);
			break;
		case REVEAL_MINE:
			reveal_mines(x, y, size, board);
			if (is_mine(&board[x][y]))
				gameover = true;
			break;
		case QUIT:
			gameover = true;
			break;
		case UNDEFINED:
			break;
		}
	}
	reveal_board(size, board);
	printd(size, board);

	getch();
	exitd();
	free(board);
}

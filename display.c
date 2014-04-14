#include <ncurses.h>
#include "display.h"
#include "mine.h"

DisplayMode mode;

int startd(DisplayMode _mode)
{/*
	mode = _mode;
	initscr();
	printw("hello world");
	refresh();
	getch();*/
	return 0;
}

int exitd(void)
{
	endwin();
	return 0;
}

void printd(int size, Mine **board)
{/*
	printw("yes");
	getch();*/

	printf("    ");
	for(char header  = 'a'; header - 'a' < size; header++) {
		printf("%c ", header);
	}
	printf("\n");

	for(int y = 0; y < size; y++) {
		printf("%3d ", y + 1);
        for(int x = 0; x < size; x++) {
			if(board[x][y].adj_mines == 9)
				printf("# ");
			else
            	printf("%d ", board[x][y].adj_mines);
        }
        printf("\n");
    }
}

void getin(int *x, int *y)
{
	//
}

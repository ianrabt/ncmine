#include <ncurses.h>
#include <string.h>
#include "display.h"
#include "mine.h"

int startd()
{
	char *title = "MINESWEEPER";
	char *sub   = "by Ian Taylor";
	char *start = "press any key to start";
	int row, col;

	initscr();
	getmaxyx(stdscr, row, col);
	attron(A_BOLD);
	mvprintw(row/2-1, (col-strlen(title))/2, "%s", title);
	attroff(A_BOLD);
	mvprintw(row/2,   (col-strlen(title))/2, "%s", sub);
	mvprintw(row*2/3, (col-strlen(start))/2, "%s", start);
	refresh();
	getch();
	return 0;
}

int exitd(void)
{
	endwin();
	return 0;
}

void printd(int size, Mine **board)
{
	clear();
	printw("yes");
	getch();
}

void getin(int *x, int *y)
{
}

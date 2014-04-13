#include <ncurses.h>
#include "display.h"
#include "mine.h"

int startd(void)
{
	initscr();
	printw("hello world");
	refresh();
	getch();
	return 0;
}

int exitd(void)
{
	endwin();
	return 0;
}

void printd(int size)
{
	printw("yes");
	getch();
}

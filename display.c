#include <ncurses.h>
#include <string.h>
#include "display.h"
#include "mine.h"

WINDOW *board_win;
WINDOW *info_win;

void mvwincenter(WINDOW *win);
int startd(int size)
{
	char *title = "MINESWEEPER";
	char *sub   = "by Ian Taylor";
	char *start = "press any key to start";
	int row, col;

	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();
	// start screen:
	getmaxyx(stdscr, row, col);
	attron(A_BOLD);
	mvprintw(row/2-1, (col-strlen(title))/2, "%s", title);
	attroff(A_BOLD);
	mvprintw(row/2,   (col-strlen(title))/2, "%s", sub);
	mvprintw(row*2/3, (col-strlen(start))/2, "%s", start);
	refresh();
	getch();

	// set up windows:
	clear();
	refresh();
	// set proper dimensions, with room for the boarder.
	board_win = newwin(2 + size, 1 + (size * 2), 0, 0);
	mvwincenter(board_win);
	box(board_win, 0 , 0);
	return 0;
}

void mvwincenter(WINDOW *win)
{
	int max_x, max_y, win_x, win_y;
	getmaxyx(stdscr, max_x, max_y);
	getmaxyx(win, win_x, win_y);

	mvwin(board_win, max_x/2 - win_x/2, max_y/2 - win_y/2); 
}

int exitd(void)
{
	delwin(board_win);
	endwin();
	return 0;
}

char getminech(Mine* mine)
{
	if (mine->visible) {
		return mine->adj_mines + (int) '0';
	} else {
		return ' ';
	}
}

void printd(int size, Mine **board)
{
	// reset window position to center
	clear();
	mvwincenter(board_win);
	refresh();

	mvwprintw(board_win, 0, 0, "\n");
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			wprintw(board_win, " %c", getminech(&board[x][y]));
		}
		wprintw(board_win, "\n");
	}

	// draw boarders
	box(board_win, 0, 0);
	wrefresh(board_win);
}

static void offsetcur(int offy, int offx)
{
	int x, y;
	getyx(stdscr, y, x);
	move(y + offy, x + offx);
}


void getin(int *x, int *y)
{
	// put cursor in board window
	{
		int winx, winy;
		getbegyx(board_win, winy, winx);
		move(winy + 1, winx + 1);
	}

	int in;
	do {
		in = getch();
		offsetcur(0, 0);
		switch(in)
		{
			case KEY_LEFT:
				offsetcur(0, -2);
				break;
			case KEY_RIGHT:
				offsetcur(0, 2);
				break;
			case KEY_UP:
				offsetcur(-1, 0);
				break;
			case KEY_DOWN:
				offsetcur(1, 0);
				break;
		}
	} while (in > '9' || in < '0');
	*x = in - '0';
	*y = 0;
}

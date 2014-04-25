#include <ncurses.h>
#include <string.h>
#include "display.h"
#include "mine.h"

DisplayMode mode;

int startd(DisplayMode _mode)
{
	mode = _mode;
	switch(mode) {
		case TEXT:
			// do nothing
			break;
		case NCURSES:
			// declarations do not count as statements...need an empty one.
			;
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
			break;
	}

	return 0;
}

int exitd(void)
{
	switch(mode) {
		case TEXT:
			// do nothing
			break;
		case NCURSES:
			endwin();
			break;
	}
	return 0;
}

void printd(int size, Mine **board)
{
	switch(mode) {
		case TEXT:
			printf("    ");
			for(char header  = 'a'; header - 'a' < size; header++) {
				printf("%c ", header);
			}
			printf("\n");

			for(int y = 0; y < size; y++) {
				printf("%3d ", y + 1);
        		for(int x = 0; x < size; x++) {
					if(board[x][y].visible) {
						if(board[x][y].adj_mines == 9)
							printf("@ ");
						else if(board[x][y].adj_mines == 0)
							printf("  ");
						else
							printf("%d ", board[x][y].adj_mines);
					} else {
						printf("# ");
					}
        		}
				printf("\n");
			}
			break;

		case NCURSES:
			clear();
			printw("yes");
			getch();
			break;
	}
}

void getin(int *x, int *y)
{
	input_loop: // infinite
	{
		*x = -1; *y = -1;
		char input[BUFSIZ];
		printf("> ");
		if(fgets(input, sizeof(input), stdin) != NULL) {
			char *p;
			// trim '\n' from end of the string.
			if((p = strchr(input, '\n')) != NULL) {
				*p = '\0';
			}
			
			int i;
			for(int i = 0; input[i]; i++) {
				if(input[i] >= 'a' && input[i] <= 'a' + size) {
					if(i == 0)
						goto input_loop;
					break;
				}

				if(input[i] < '0' || input[i] > '1' + size) {
					goto input_loop;
				}
			}

			int y_val = atoi(input);
			char *x_val = &input[i];
			if(y_val > 0 && y_val <= size) {
				if(*x_val >= 'a' && *x_val < 'a' + size) {
					*x = (int) *x_val - 'a';
					*y = y_val - 1;
					goto exit_loop; // input is finally valid
                }
            }
		}

		goto input_loop;
	}
	exit_loop:
}

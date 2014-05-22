#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "display.h"
#include "mine.h"

// misc. state variables:
static WINDOW *board_win; // main window
static WINDOW *info_win; // TODO: window with help info.
static int prev_y = 0, prev_x = 0; // used for resetting the cursor position
			// after calling printd or redrawing.

// color variables:
static bool color_output = false; // defaults to true if supported when set later by startd.
enum colors {
    FLAGGED,    // the mine is flagged
    MINE,       // is a mine. only used on game over.
    LOW,        // low number of adj mines.
    MED,        // medium number of adj mines.
    HIGH        // high number of adj mines.
};

/**
 * Resets the cursor to its previous position. set_cur must have been called
 * beforehand for this method to work as intended. 
 */
void reset_cur(void)
{
	move(prev_y, prev_x);
}

/**
 * Saves the cursor's position. The position may then be reset by reset_cur.
 */
void set_cur(void)
{
	getyx(stdscr, prev_y, prev_x);
}

/**
 * Turns color output on and off. Will take a board refresh (call to printd) to
 * update. This method will not enable color output if the terminal does not
 * support it.
 */
void set_color_output(bool enabled)
{
	if (has_colors())
		color_output = enabled;
}

void move_cur_center(void)
{
	getbegyx(board_win, prev_y, prev_x);
	prev_y++;
	prev_x++;
	reset_cur();
}

void mvwincenter(WINDOW *win);
static void init_colors(void);

/**
 * Starts the ncurses display, as well as some essential variables and
 * settings. Must be called to use any other methods.
 *
 * @return: non zero value on failure.
 */
int startd(int size)
{
	char *title = "MINESWEEPER";
	char *sub   = "by Ian Taylor";
	char *start = "press any key to start";
	int row, col;

	// set some settings
	initscr();
	color_output = has_colors();
	if (color_output) // don't set up colors if they're not supported.
		init_colors();
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

	move_cur_center();

	return 0;
}

void mvwincenter(WINDOW *win)
{
	assert(win != NULL);

	int max_x, max_y, win_x, win_y;
	getmaxyx(stdscr, max_x, max_y);
	getmaxyx(win, win_x, win_y);

	mvwin(board_win, max_x/2 - win_x/2, max_y/2 - win_y/2); 
}

/**
 * TODO: implement
 */
static void init_colors(void)
{
    start_color();

    init_pair(FLAGGED, COLOR_RED, COLOR_BLACK);
    init_pair(MINE, COLOR_RED, COLOR_BLACK);
    init_pair(LOW, COLOR_RED, COLOR_BLACK);
    init_pair(MED, COLOR_RED, COLOR_BLACK);
    init_pair(HIGH, COLOR_RED, COLOR_BLACK);
}

/**
 * Cleans up and exits ncurses.
 *
 * @return: non zero value on failure.
 */
int exitd(void)
{
	delwin(board_win);
	endwin();
	return 0;
}

static void setmineattr(Mine* mine)
{
	if (color_output) {
		// TODO: implement
	}
}

char getminech(Mine* mine)
{
	assert(mine != NULL);

	if (mine->visible) {
		if (IS_MINE(*mine)){
			return '@';
		} else {
			return mine->adj_mines + (int) '0';
		}
	} else if(mine->flagged) {
		return 'f';
	} else {
		return ' ';
	}
}

void printd(int size, Mine **board)
{
	assert(board != NULL);

	// reset window position to center
	clear();
	mvwincenter(board_win);
	refresh();

	mvwprintw(board_win, 0, 0, "\n");
	for (int y = 0; y < size; y++) {
		assert(board[y] != NULL);
		for (int x = 0; x < size; x++) {
			Mine* mine = &board[x][y];
			setmineattr(mine);
			wprintw(board_win, " %c", getminech(mine));
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

static void get_bounds(WINDOW *win, int *y1, int *x1, int *y2, int *x2)
{
	assert(win != NULL);
	assert(y1 != NULL);
	assert(x1 != NULL);
	assert(y2 != NULL);
	assert(x2 != NULL);

	// top left corner of the window:
	int ybeg, xbeg;
	getbegyx(win, ybeg, xbeg);
	// bottom right corner:
	int yend, xend;
	getmaxyx(win, yend, xend);
	yend += ybeg; xend += xbeg; // getmaxyx returns size of win, not coordinates
	*y1 = ybeg; *x1 = xbeg;
	*y2 = yend; *x2 = xend;
}

/**
 * assumes that window has a border that may not be overwritten
 */
static bool is_in_win(WINDOW *win, int y, int x)
{
	assert(win != NULL);

	// top left corner of the window:
	int ybeg, xbeg, yend, xend;
	get_bounds(win, &ybeg, &xbeg, &yend, &xend);

	return (y > ybeg && x > xbeg) && (y < yend - 1 && x < xend - 1);
}

void move_board_cur(int offy, int offx)
{
	int ycur, xcur;
	getyx(stdscr, ycur, xcur);
	ycur += offy;
	xcur += offx;
	if (is_in_win(board_win, ycur, xcur))
		offsetcur(offy, offx);
}

/**
 * @return: TODO
 */
void get_board_yx(int *y, int *x)
{
	assert(y != NULL);
	assert(x != NULL);

	int ycur, xcur;
	getyx(stdscr, ycur, xcur);
	int ywin, xwin;
	getbegyx(board_win, ywin, xwin);
	
	*y = ycur - ywin - 1;
	*x = (xcur - xwin - 1)/2;
}
	
/**
 * Gets user input. Specifically mine coordinates and an opperation to apply to
 * those coordinates.
 * 
 * precondition: x and y aren't NULL.
 *
 * postcondition: *x and *y are set to valid coordinates. getin will continue
 * 	to prompt for input until the user supplied is valid.
 */
enum operation getin(int *x, int *y)
{
	assert(y != NULL);
	assert(x != NULL);

	reset_cur();
	enum operation op = UNDEFINED;
	do {
		int in = getch();
		offsetcur(0, 0);
		switch(in) {
		case 'h':
		case KEY_LEFT:
			move_board_cur(0, -2);
			break;
		case 'l':
		case KEY_RIGHT:
			move_board_cur(0, 2);
			break;
		case 'k':
		case KEY_UP:
			move_board_cur(-1, 0);
			break;
		case 'j':
		case KEY_DOWN:
			move_board_cur(1, 0);
			break;
		case ' ':
		case '\r':
		case '\n':
			op = REVEAL_MINE;
			break;
		case 'f':
			op = TOGGLE_FLAG;
			break;
		case 'q':
			op = QUIT;
			break;
		}

	} while (op == UNDEFINED);

	get_board_yx(y, x);
	set_cur();
	return op;
}

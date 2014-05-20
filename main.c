#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <unistd.h>
#include "display.h"
#include "mine.h"

// help info strings:
static char* usage =
	"Ncurses Minesweeper by Ian Taylor\n"
	"usage: ncmine [-hvc] [SIZE]\n";
	
static char* help  =
	"flags:\n"
	"\t-h : print this information\n"
	"\t-v : print version information\n"
	"\t-c : disable color output\n"
	"[SIZE]:"
	"\tThe board size as an integer. Defaults to 10.\n";

static char* version =
	"Ncurses Minesweeper: 0.2";

// runtime options:
static bool color_output = true; // defaults to true, display.c will disable color output if unsupported.
static int size = 15;

/**
 * Parses command line arguments.
 *
 * precondition: 
 */
static void parse_args (int argc, char** argv)
{
	assert(argv != NULL);
//	assert(argc < 0);

	char* size_value = NULL;
	int c;
	
	opterr = 0;
	
	while ((c = getopt(argc, argv, "hvc")) != -1) {
		switch (c) {
		case 'h':
			printf("%s\n%s", usage, help);
			exit(0);
		case 'v':
			printf("%s\n", version);
			exit(0);
		case 'c':
			color_output = false;
			break;
		case '?':
			if (isprint(optopt))
				fprintf(stderr, "Unknown option '-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
			exit(1);
		default:
			abort();
		}
	}
	if (optind < argc) {
		size_value = argv[optind];
		size = atoi(size_value);
	}
}

int main (int argc, char** argv)
{
	parse_args(argc, argv);
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

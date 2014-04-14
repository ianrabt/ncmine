#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "mine.h"

int main (int argc, char** argv)
{
	int size = 5;
	Mine **board = create_board(size, .5);
	startd(TEXT);
	printd(size, board);
	exitd();
	free(board);
}

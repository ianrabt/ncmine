#ifndef DISPLAY_H
#define DISPLAY_H
#include "mine.h"

typedef enum {TEXT, NCURSES} DisplayMode;

int startd(DisplayMode mode);
int exitd(void);
void printd(int size, Mine **board);

void getin(int *x, int *y);

#endif

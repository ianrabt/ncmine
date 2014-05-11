#ifndef DISPLAY_H
#define DISPLAY_H
#include "mine.h"

int startd();
int exitd(void);
void printd(int size, Mine **board);

enum operation getin(int *x, int *y);

#endif

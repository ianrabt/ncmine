#ifndef DISPLAY_H
#define DISPLAY_H
#include "mine.h"

void set_color_output(bool enabled);

int startd();
int exitd(void);
void printd(int size, Mine **board);

enum operation getin(int *x, int *y);

#endif

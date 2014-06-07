#ifndef DISPLAY_H
#define DISPLAY_H
#include "mine.h"

int startd();
int exitd(void);
void printd(int size, Mine **board);

struct instruction getin(void);

#endif

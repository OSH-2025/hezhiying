#ifndef _SIGNAL_H
#define _SIGNAL_H

#include "lightrt.h"

#define MAX_SIGNALS 16

#define SIGNAL_EMPTY 0
#define SIGNAL_INACTIVE 1
#define SIGNAL_ACTIVE 2

typedef struct
{
    int state;
} signal_t;

extern signal_t signals[MAX_SIGNALS];

void signal_new(int id);
void signal_active(int id);
void signal_inactive(int id);
void signal_dispose(int id);
int signal_find_empty();

#endif

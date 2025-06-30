/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

timer.u.h - User header for system timing

*/

#ifndef _TIMER_U_H
#define _TIMER_U_H

#include "../sys/global.h"
#include <stdint.h>

typedef uint64_t tick;

typedef int64_t timespan;

int now(tick *t);

#endif

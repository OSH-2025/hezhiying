/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

timer.u.h - User header for system timing

*/

#ifndef _TASK_U_H
#define _TASK_U_H

#include "../sys/global.h"

typedef struct
{
    uint ticklow;
    uint tickhigh;
} tick;

typedef struct
{
    uint spanlow;
    int spanhigh;
} timespan;

int now(tick *t);

int timeadd(tick *t, timespan *span);
int timesub(tick *ta, tick *tb, timespan *span);
int timeneg(timespan *t);
int timetest(timespan *t, int *result);

#endif

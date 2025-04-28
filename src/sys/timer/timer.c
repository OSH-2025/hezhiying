
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

timer.c - timer event handler

*/

#include "timer.h"

tick uptime;

int now(tick *t)
{
    *t = uptime;

    return STATUS_SUCCESS;
}

void timer_1ms()
{
    // Update system uptime
    uptime++;
}

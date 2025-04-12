
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

timer.c - timer event handler

*/

#include "timer.h"

uint ticklow = 0, tickhigh = 0;

void timer1ms()
{
    ++ticklow;
    if(ticklow == 0)
    {
        ++tickhigh;
    }
}

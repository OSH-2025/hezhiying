/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

task.s.h - System header for task manipulation

*/

#ifndef _TASK_S_H
#define _TASK_S_H

#include "../../uinc/task.u.h"

// For waiting for signals
int tsetsigwait(int pid, int sig);
int tdelwait(int pid);

#endif

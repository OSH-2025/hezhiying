/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~
io.h - IO 设备控制

*/

#ifndef _IO_H
#define _IO_H

#include "../global.h"
#include "../../uinc/io.u.h"
#include "../task/task.s.h"

typedef uint32_t buzzer_freq;

int _io_init(void);

int _buzzer_set(buzzer_freq freq);

int _led_set(int state);

int _key_get(int *state);

#endif

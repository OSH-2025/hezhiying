/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~
io.s.h - IO 设备控制系统端口

*/

#ifndef _IO_U_H
#define _IO_U_H

#include "../sys/global.h"

LRT_SAPI int _io_init(void);
LRT_SAPI int _buzzer_set(buzzer_freq freq);
LRT_SAPI int _led_set(int state);
LRT_SAPI int _key_get(int *state);

#endif

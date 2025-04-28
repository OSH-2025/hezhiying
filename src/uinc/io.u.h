/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~
io.u.h - IO 设备控制用户端口

*/

#ifndef _IO_U_H
#define _IO_U_H

#include "../sys/global.h"

LRT_UAPI int _io_init(void);
LRT_UAPI int _buzzer_set(buzzer_freq freq);
LRT_UAPI int _led_set(int state);
LRT_UAPI int _key_get(int *state);

#endif

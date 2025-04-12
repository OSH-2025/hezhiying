/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

global.h - Global definitions and constants

*/

#ifndef _GLOBAL_H
#define _GLOBAL_H

// Targets
#define STM32              0

// Global constants
#define LRT_VERSION        1
#define LRT_TARGET         STM32

// Routine flags
#define LRT_UAPI // User side system API

// Normal types
typedef unsigned int uint; // By default, 32 bits.

// Utility macros
#define MAKEBIT(n) (1 << (n)) // Make bit n to 1
#define NOTIMPL (This must be error if used! & !) // Not implemented

// Return values
#define STATUS_SUCCESS                0   // Successfully done
#define STATUS_NOT_IMPLEMENTED        -1  // Not implemented
#define STATUS_RESOURCE_INSUFFICIENT  -2  // Resource run out

#endif

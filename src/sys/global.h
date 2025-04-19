/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

global.h - Global definitions and constants

*/

#ifndef _GLOBAL_H
#define _GLOBAL_H

// Targets
#define STM32 0

// Global constants
#define LRT_VERSION 1
#define LRT_TARGET STM32

// Routine flags
#define LRT_UAPI // User side system API
#define LRT_SAPI // System side system API

// Normal types
typedef unsigned int uint; // By default, 32 bits.

// Utility macros
#define MAKEBIT(n) (1 << (n))                    // Make bit n to 1
#define NOTIMPL (This must be error if used !&!) // Not implemented

// System object ID check
#define IDWRONG(id, type) (((id) < 0) || ((id) >= MAX_##type##_ITEM)) //

// Return values
#define STATUS_SUCCESS 0                // Successfully done
#define STATUS_NOT_IMPLEMENTED -1       // Not implemented
#define STATUS_RESOURCE_INSUFFICIENT -2 // Resource run out
#define STATUS_INVALID_ID -3            // Invalid object ID
#define STATUS_INCORRECT_STATUS -4      // Incorrect status: operation specified and system object status
#define STATUS_INVALID_PARAMETER -5     // Incorrect parameter

#endif

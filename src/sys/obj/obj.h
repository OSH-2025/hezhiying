
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

obj.h - system object management

*/

#include "../global.h"

// Handle typedef
typedef uint handle;
typedef struct
{
    int type;
    int usagecount;

    int itable;
} ihandle;

// System object types
#define OBJTYPE_EMPTY 0
#define OBJTYPE_TASK 1
#define OBJTYPE_COMM 2
#define OBJTYPE_SIGNAL 3
#define OBJTYPE_REF 100 // Where ihandle->itable will be another handle

#define MAX_HANDLE_ITEM 100

extern ihandle handles[MAX_HANDLE_ITEM];

// Create object = map a handle to a internal subsystem table
int _xnewobj(int itable, int type, int *h);

// Close handle
int _xcloseobj(int h);

// Dulipicate handle (make reference to the same system object)
int _xrefobj(int h, int *newhandle);

// Check handle invalid?
int _xvalidh(int h);

// Check object type, pass OBJTYPE_* macro to objtype.
int _xistypeh(int h, int objtype);

// Get ID
int _xgetid(int h, int *id);

// Remote object
int _xremoteobj(int uid, int h, int *localh);

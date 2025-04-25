
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

obj.c - system object management

*/

#include "obj.h"

// Global handle table
ihandle handles[MAX_HANDLE_ITEM];

// Create object
int _xnewobj(int itable, int type, int *h)
{
    int newh = -1;

    for (int i = 0; i < MAX_HANDLE_ITEM; ++i)
    {
        if (handles[i].type == OBJTYPE_EMPTY)
        {
            newh = i;
            break;
        }
    }

    if (newh == -1)
    {
        return STATUS_RESOURCE_INSUFFICIENT;
    }

    handles[newh].itable = itable;
    handles[newh].type = type;
    handles[newh].usagecount = 1;

    *h = newh;

    return STATUS_SUCCESS;
}

// Close handle
int _xcloseobj(int h)
{
    if (_xvalidh(h) != STATUS_SUCCESS)
    {
        return STATUS_INVALID_ID;
    }

    handles[h].usagecount--;

    if (handles[h].usagecount == 0)
    {
        // Delete the handle
        handles[h].type = OBJTYPE_EMPTY;
        handles[h].itable = -1;
    }

    return STATUS_SUCCESS;
}

// Dulipicate handle (make reference to the same system object)
int _xrefobj(int h, int *newhandle)
{
    if (_xvalidh(h) != STATUS_SUCCESS)
    {
        return STATUS_INVALID_HANDLE;
    }

    return _xnewobj(h, OBJTYPE_REF, newhandle);
}

// Check handle invalid?
int _xvalidh(int h)
{
    if (h >= 0 && h < MAX_HANDLE_ITEM)
        return STATUS_SUCCESS;

    return STATUS_INVALID_HANDLE;
}

// Check object type, pass OBJTYPE_* macro to objtype.
int _xistypeh(int h, int objtype)
{
    if (_xvalidh(h) != STATUS_SUCCESS)
        return STATUS_INVALID_HANDLE;
    else if (handles[h].type == objtype)
        return STATUS_SUCCESS;

    return STATUS_INCORRECT_HANDLE_TYPE;
}

int _xgetid(int h, int *id)
{
    if (_xvalidh(h) != STATUS_SUCCESS)
    {
        return STATUS_INVALID_HANDLE;
    }

    return handles[h].itable;
}

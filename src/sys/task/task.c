
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

task.c - task manipulation

*/

#include "task.h"

int exec(entryproc p, void* param, const startinf* si)
{
    // Find an empty slot
    int emptyslot = -1;

    for(int i = 0; i < MAX_TASK_ITEM; ++i)
    {
        if(tsklist[i].pid == -1)
        {
            // Found one empty slot
            emptyslot = i;
            break;
        }
    }

    if(emptyslot == -1)
    {
        // Cannot find an empty task slot
        return STATUS_RESOURCE_INSUFFICIENT;
    }

    // TODO: not implemented.

    return STATUS_NOT_IMPLEMENTED;
}

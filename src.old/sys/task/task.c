
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

task.c - task manipulation

*/

#include "task.h"

task tsklist[MAX_TASK_ITEM];
int currentpid = -1;

int exec(entryproc p, void *param, const startinf *si)
{
    if (PTRWRONG(si))
    {
        return STATUS_INVALID_PARAMETER;
    }

    // Find an empty slot
    int emptyslot = -1;

    for (int i = 0; i < MAX_TASK_ITEM; ++i)
    {
        if (tsklist[i].pid == -1)
        {
            // Found one empty slot
            emptyslot = i;
            break;
        }
    }

    if (emptyslot == -1)
    {
        // Cannot find an empty task slot
        return STATUS_RESOURCE_INSUFFICIENT;
    }

    // TODO: not implemented.

    return STATUS_NOT_IMPLEMENTED;
}

int swtch(int npid)
{
    // Save up current context
}

int cpid(int *pid)
{
    *pid = currentpid;

    return STATUS_SUCCESS;
}

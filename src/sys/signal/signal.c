//
// Created by Administrator on 25-4-17.
//

#include "signal.h"

#include "../timer/timer.h"
#include "../task/task.h"
#include "../obj/obj.h"

signl siglist[MAX_SIGNAL_ITEM];

int signew(uint flag, int *sig)
{
    if (PTRWRONG(sig))
    {
        return STATUS_INVALID_PARAMETER;
    }

    int newsig = -1;

    for (int i = 0; i < MAX_SIGNAL_ITEM; ++i)
    {
        if (siglist[i].status == SIG_EMPTY)
        {
            newsig = i;
            break;
        }
    }

    if (newsig == -1)
    {
        return STATUS_NOT_IMPLEMENTED;
    }

    siglist[newsig].status = SIG_INACTIVE;
    siglist[newsig].flag = flag;

    return _xnewobj(newsig, OBJTYPE_SIGNAL, sig); // TODO: what if handle table is full? the signal is created but there's no way to access it.
}

int sigclose(int sig)
{
    if (INVALID_SIGNAL(sig))
    {
        return STATUS_INVALID_ID;
    }

    siglist[sig].status = SIG_EMPTY;
    return STATUS_SUCCESS;
}

int sigset(int sig, int s)
{
    if (INVALID_SIGNAL(sig))
    {
        return STATUS_INVALID_ID;
    }

    if (s == SIG_EMPTY)
    {
        return STATUS_INVALID_PARAMETER;
    }

    siglist[sig].status = s;
    return STATUS_SUCCESS;
}

int waitsig(int sig, int timeout)
{
    // Should be implemented mainly in task subsystem
    return STATUS_NOT_IMPLEMENTED;
}

int waitany(const int *sig, int nsig, int timeout)
{
    // Should be implemented mainly in task subsystem
    return STATUS_NOT_IMPLEMENTED;
}

int readsig(int sig)
{
    return siglist[sig].status;
}

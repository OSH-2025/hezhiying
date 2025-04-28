//
// Created by Administrator on 25-4-17.
//

#include "signal.h"

#include "../timer/timer.h"
#include "../task/task.h"

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
        return STATUS_RESOURCE_INSUFFICIENT;
    }

    siglist[newsig].status = SIG_INACTIVE;
    siglist[newsig].flag = flag;

    *sig = newsig;
    return STATUS_SUCCESS;
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

int waitlo(int sig, int timeout)
{
    tick begin;
    int rv;

    if ((rv = now(&begin)) != STATUS_SUCCESS)
    {
        return rv;
    }

    while (true)
    {
        tick tnow;
        now(&tnow);

        if (tnow - begin > timeout)
        {
            return STATUS_WAIT_TIMEOUT;
        }

        int status;
        if ((rv = readsig(sig, &status)) != STATUS_SUCCESS)
        {
            return rv;
        }

        if (status != SIG_INACTIVE)
        {
            return STATUS_SUCCESS;
        }

        yield();
    }
}

int readsig(int sig, int *status)
{
    *status = siglist[sig].status;

    return STATUS_SUCCESS;
}

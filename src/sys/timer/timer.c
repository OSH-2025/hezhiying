
/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

timer.c - timer event handler

*/

#include "timer.h"

tick uptime;

int now(tick *t)
{
    t->ticklow = uptime.ticklow;
    t->tickhigh = uptime.tickhigh;

    return STATUS_SUCCESS;
}

int timeadd(tick *t, timespan *span)
{
    if (PTRWRONG(t) || PTRWRONG(span))
    {
        return STATUS_INVALID_PARAMETER;
    }

    // Add the low parts first
    uint old_ticklow = t->ticklow;
    t->ticklow += span->spanlow;

    // Detect carry: if the new ticklow is less than the old one, a carry occurred
    uint carry = (t->ticklow < old_ticklow) ? 1 : 0;

    // Add high part and carry
    t->tickhigh += span->spanhigh + carry;

    return STATUS_SUCCESS;
}

int timeneg(timespan *t)
{
    if (PTRWRONG(t))
    {
        return STATUS_INVALID_PARAMETER;
    }

    t->spanhigh = -t->spanhigh;

    return STATUS_SUCCESS;
}

int timetest(timespan *t, int *result)
{
    if (PTRWRONG(t) || PTRWRONG(result))
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (t->spanhigh > 0)
    {
        *result = 1;
    }
    else if (t->spanhigh < 0)
    {
        *result = -1;
    }
    else
    {
        *result = 0;
    }

    return STATUS_SUCCESS;
}

int timesub(tick *ta, tick *tb, timespan *span)
{
    if (PTRWRONG(ta) || PTRWRONG(tb) || PTRWRONG(span))
    {
        return STATUS_INVALID_PARAMETER;
    }

    int sign;

    if (ta->tickhigh > tb->tickhigh ||
        (ta->tickhigh == tb->tickhigh && ta->ticklow >= tb->ticklow))
    {
        // ta >= tb → result is positive
        sign = 1;
        if (ta->ticklow >= tb->ticklow)
        {
            span->spanlow = ta->ticklow - tb->ticklow;
            span->spanhigh = (int)(ta->tickhigh - tb->tickhigh);
        }
        else
        {
            // borrow from high
            span->spanlow = (uint)(ta->ticklow - tb->ticklow);
            span->spanlow += (1ULL << 32); // compensate for underflow
            span->spanhigh = (int)(ta->tickhigh - tb->tickhigh - 1);
        }
    }
    else
    {
        // ta < tb → result is negative
        sign = 0;
        if (tb->ticklow >= ta->ticklow)
        {
            span->spanlow = tb->ticklow - ta->ticklow;
            span->spanhigh = (int)(tb->tickhigh - ta->tickhigh);
        }
        else
        {
            // borrow from high
            span->spanlow = (uint)(tb->ticklow - ta->ticklow);
            span->spanlow += (1ULL << 32); // compensate for underflow
            span->spanhigh = (int)(tb->tickhigh - ta->tickhigh - 1);
        }

        // Now negate the result to get a normalized negative timespan
        span->spanlow = ~span->spanlow + 1;
        if (span->spanlow == 0)
        {
            span->spanhigh = -span->spanhigh;
        }
        else
        {
            span->spanhigh = ~span->spanhigh;
        }
    }

    return STATUS_SUCCESS;
}

void timer_1ms()
{
    // Update system uptime
    uptime.ticklow++;

    if (uptime.ticklow == 0)
    {
        uptime.tickhigh++;
    }
}

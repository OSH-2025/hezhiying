//
// Created by Administrator on 25-4-17.
//

#ifndef _SIGNAL_H
#define _SIGNAL_H

#include "../global.h"
#include "../../uinc/signal.u.h"
#include "../task/task.s.h"

#define SIG_EMPTY -1
#define SIG_INACTIVE 0
#define SIG_ACTIVE 1

typedef struct
{
    int status;
    uint flag;
} signl;

extern signl siglist[MAX_SIGNAL_ITEM];

#define INVALID_SIGNAL(sig) (IDWRONG(sig, SIGNAL) || siglist[sig].status == SIG_EMPTY)

#endif

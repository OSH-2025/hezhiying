//
// Created by Administrator on 25-4-17.
//

#ifndef _SIGNAL_U_H
#define _SIGNAL_U_H

#include "../sys/global.h"

#define MAX_SIGNAL_ITEM 100

#define SIG_FLAG_DEFAULT 0
#define SIG_FLAG_AUTORESET MAKEBIT(1)

LRT_UAPI int signew(uint flag, int *sig);
LRT_UAPI int sigclose(int sig);
LRT_UAPI int sigset(int sig, int status);
LRT_UAPI int waitlo(int sig, int timeout);
LRT_UAPI int waitck(int sig, int timeout);
LRT_UAPI int waitrt(int sig, int timeout);
LRT_UAPI int waitany(const int *sig, int nsig, int timeout);
LRT_UAPI int readsig(int sig, int *status);

#endif

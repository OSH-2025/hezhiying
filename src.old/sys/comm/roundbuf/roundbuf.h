/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

roundbuf.h - Round shaped dual-direction queue buffer, with chunk control, for streaming communication

*/

#ifndef _ROUNDBUF_H
#define _ROUNDBUF_H

#include "../global.h"

typedef struct
{
    uint size;
} bufchunk;

typedef struct
{
    char *pbuf;
    char *phead;
    char *ptail;

    uint size;
} roundbuf;

void rbcreat(char *pbuf, uint size, roundbuf *pb);

void rbheadpush(const char *pdata, uint size, roundbuf *pb);
void rbheadcp(char *dst, uint dstsize, uint *cpsize);
void rbheadpop();

void rbtailpush(const char *pdata, uint size, roundbuf *pb);
void rbtailcp(char *dst, uint dstsize, uint *cpsize);
void rbtailpop();

#endif

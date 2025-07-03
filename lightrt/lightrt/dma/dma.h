#include "../lightrt.h"

#ifndef _DMA_H
#define _DMA_H

void dma_init();
void dma_config(void *src, void *dst, int cblk, int szblk);
void dma_start();
int dma_status();
void dma_dispose();

#endif
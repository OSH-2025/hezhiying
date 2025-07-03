#include "../lightrt.h"

#ifndef _MM_H
#define _MM_H

typedef struct
{
    void *ptr;
    int size;
    int allocated;
} memblk;

typedef struct
{
    void *memory_pool; // Pointer to start of memory
    int memory_size;   // Total size
    memblk *blocks;    // Array of block descriptors
    int block_count;   // Current number of blocks
    int max_blocks;    // Max blocks allowed (capacity of `blocks`)
} memctx;

void mem_init(memctx *ctx, void *memory_pool, int memory_size, memblk *block_list, int max_blocks);
void *mem_alloc(memctx *ctx, int size);
void mem_free(memctx *ctx, void *ptr);
void mem_dump(const memctx *ctx);

#endif
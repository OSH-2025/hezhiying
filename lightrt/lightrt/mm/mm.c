#include "mm.h"
#include <stddef.h>

void mem_init(memctx *ctx, void *memory_pool, int memory_size, memblk *block_list, int max_blocks)
{
    ctx->memory_pool = memory_pool;
    ctx->memory_size = memory_size;
    ctx->blocks = block_list;
    ctx->block_count = 1;
    ctx->max_blocks = max_blocks;

    ctx->blocks[0].ptr = memory_pool;
    ctx->blocks[0].size = memory_size;
    ctx->blocks[0].allocated = 0;

    // memset(memory_pool, 0, memory_size);
}

void *mem_alloc(memctx *ctx, int size)
{
    for (int i = 0; i < ctx->block_count; i++)
    {
        memblk *blk = &ctx->blocks[i];
        if (!blk->allocated && blk->size >= size)
        {
            if (blk->size > size && ctx->block_count < ctx->max_blocks)
            {
                // Split block
                for (int j = ctx->block_count; j > i + 1; j--)
                {
                    ctx->blocks[j] = ctx->blocks[j - 1];
                }

                ctx->blocks[i + 1].ptr = (char *)blk->ptr + size;
                ctx->blocks[i + 1].size = blk->size - size;
                ctx->blocks[i + 1].allocated = 0;

                blk->size = size;
                ctx->block_count++;
            }
            blk->allocated = 1;
            return blk->ptr;
        }
    }
    return NULL;
}

void mem_free(memctx *ctx, void *ptr)
{
    for (int i = 0; i < ctx->block_count; i++)
    {
        memblk *blk = &ctx->blocks[i];
        if (blk->ptr == ptr && blk->allocated)
        {
            blk->allocated = 0;

            // Merge with next
            if (i + 1 < ctx->block_count &&
                !ctx->blocks[i + 1].allocated &&
                (char *)blk->ptr + blk->size == ctx->blocks[i + 1].ptr)
            {
                blk->size += ctx->blocks[i + 1].size;
                for (int j = i + 1; j < ctx->block_count - 1; j++)
                {
                    ctx->blocks[j] = ctx->blocks[j + 1];
                }
                ctx->block_count--;
            }

            // Merge with previous
            if (i > 0 &&
                !ctx->blocks[i - 1].allocated &&
                (char *)ctx->blocks[i - 1].ptr + ctx->blocks[i - 1].size == blk->ptr)
            {
                ctx->blocks[i - 1].size += blk->size;
                for (int j = i; j < ctx->block_count - 1; j++)
                {
                    ctx->blocks[j] = ctx->blocks[j + 1];
                }
                ctx->block_count--;
            }

            return;
        }
    }
}

void mem_dump(const memctx *ctx)
{
    uart_printf("Memory Dump:\n");
    for (int i = 0; i < ctx->block_count; i++)
    {
        const memblk *blk = &ctx->blocks[i];
        uart_printf("memblk %2d: Addr=%p Size=%4d %s\n", i,
                    blk->ptr,
                    blk->size,
                    blk->allocated ? "Allocated" : "Free");
    }
}

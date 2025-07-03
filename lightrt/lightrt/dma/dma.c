#include "dma.h"

void dma_init()
{
    rcc_periph_clock_enable(RCC_DMA1);
}

void dma_config(void *src, void *dst, int cblk, int szblk)
{
    // uart_printf("DMA config: %x -> %x size %d * %dbytes\n", src, dst, cblk, szblk);

    uint32_t msize, psize;
    switch (szblk)
    {
    case 1:
        msize = DMA_CCR_MSIZE_8BIT;
        psize = DMA_CCR_PSIZE_8BIT;
        break;
    case 2:
        msize = DMA_CCR_MSIZE_16BIT;
        psize = DMA_CCR_PSIZE_16BIT;
        break;
    case 4:
    default:
        msize = DMA_CCR_MSIZE_32BIT;
        psize = DMA_CCR_PSIZE_32BIT;
        break;
    }

    dma_channel_reset(DMA1, DMA_CHANNEL1);

    // Set addresses
    dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)dst);     // Destination
    dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)src); // Source

    dma_set_number_of_data(DMA1, DMA_CHANNEL1, cblk);

    // Set configuration
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1); // Source = "peripheral"
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
    dma_enable_peripheral_increment_mode(DMA1, DMA_CHANNEL1);

    dma_set_memory_size(DMA1, DMA_CHANNEL1, msize);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL1, psize);

    dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_MEDIUM);

    DMA_CCR(DMA1, DMA_CHANNEL1) |= DMA_CCR_MEM2MEM;
}

void dma_start()
{
    uart_printf("DMA1 working.\n");
    dma_enable_channel(DMA1, DMA_CHANNEL1);
    uart_printf("DMA_START finished.\n");
}

int dma_status()
{
    int status = dma_get_interrupt_flag(DMA1, DMA_CHANNEL1, DMA_TCIF);
    // uart_printf("DMA status %d.\n", status);
    return status;
}

void dma_dispose()
{
    dma_clear_interrupt_flags(DMA1, DMA_CHANNEL1, DMA_TCIF);
    dma_disable_channel(DMA1, DMA_CHANNEL1);
}
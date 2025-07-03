#include "../lightrt.h"
#ifndef _SVC_H
#define _SVC_H
/* This file is generated using SVC compiler. DO NOT DIRECTLY MODIFY. Edit svc.def file and run svcc instead. */
#include <stdint.h>
/* SVC Implement 10 */ uint32_t svc_task_start (task_routine routine, void* param, int id);
/* SVC Implement 11 */ uint32_t svc_task_terminate (int id);
/* SVC Implement 12 */ uint32_t svc_task_yield (int nextid);
/* SVC Implement 13 */ uint32_t svc_task_state (int id, int state);
/* SVC Implement 14 */ uint32_t svc_task_current ();
/* SVC Implement 15 */ uint32_t svc_task_prio (int id, int prio);
/* SVC Implement 20 */ uint32_t svc_clock_tick ();
/* SVC Implement 21 */ uint32_t svc_clock_delay (int wait_tick);
/* SVC Implement 30 */ uint32_t svc_signal_new (int* id);
/* SVC Implement 31 */ uint32_t svc_signal_active (int id);
/* SVC Implement 32 */ uint32_t svc_signal_inactive (int id);
/* SVC Implement 33 */ uint32_t svc_signal_dispose (int id);
/* SVC Implement 34 */ uint32_t svc_signal_wait (int id);
/* SVC Implement 35 */ uint32_t svc_signal_read (int id);
/* SVC Implement 70 */ uint32_t svc_uart_write (const char* message);
/* SVC Implement 71 */ uint32_t svc_uart_read (char* buf, int size);
/* SVC Implement 80 */ uint32_t svc_shmem_size ();
/* SVC Implement 81 */ uint32_t svc_shmem_base ();
/* SVC Implement 82 */ uint32_t svc_shmem_lock ();
/* SVC Implement 83 */ uint32_t svc_shmem_unlock ();
/* SVC Implement 90 */ uint32_t svc_rand_seed (int seed);
/* SVC Implement 91 */ uint32_t svc_rand_next ();
/* SVC Implement 100 */ uint32_t svc_gpio_led_set ();
/* SVC Implement 101 */ uint32_t svc_gpio_led_init ();
/* SVC Implement 110 */ uint32_t svc_dma_init ();
/* SVC Implement 111 */ uint32_t svc_dma_1byte (void* src, void* dst, int cblk);
/* SVC Implement 112 */ uint32_t svc_dma_2byte (void* src, void* dst, int cblk);
/* SVC Implement 113 */ uint32_t svc_dma_4byte (void* src, void* dst, int cblk);
/* SVC Implement 114 */ uint32_t svc_dma_start ();
/* SVC Implement 115 */ uint32_t svc_dma_busy ();
/* SVC Implement 116 */ uint32_t svc_dma_dispose ();
/* SVC Implement 117 */ uint32_t svc_dma_wait ();
#endif
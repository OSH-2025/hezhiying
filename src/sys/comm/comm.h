//
// Created by Administrator on 25-4-17.
//

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#endif // COMMUNICATION_H
#include <stdint.h>
#include <string.h>

#define MAX_COMM_CHANNELS 100 // 假设有最多5个通信通道
#define BUFFER_SIZE 1024      // 每个通道的缓冲区大小

// 定义通信结构体
typedef struct
{
    int uid;              // 处理器ID
    int pid;              // 过程ID
    uint8_t *buffer;      // 用于存储接收到的数据的缓冲区
    uint32_t buffer_size; // 缓冲区大小
    uint32_t data_length; // 当前缓冲区中数据的实际长度
    uint8_t is_connected; // 连接状态标志
} comm;

// 预先分配的通信通道和缓冲区
static comm channels[MAX_COMM_CHANNELS];
static uint8_t buffers[MAX_COMM_CHANNELS][BUFFER_SIZE];

int comopen(int uid, int pid, int flag, comm **f);
int comclose(comm **f);
int comrecv(comm **f, void *buf, int len, int *wlen);
int comsend(comm **f, void *buf, int len);
int comcp(comm **f, void *buf, int len, int *wlen);
int comerase(comm **f, int len);
int comavail(comm **f, int *len);
int comcheck(comm **f);
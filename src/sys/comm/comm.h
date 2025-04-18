//
// Created by Administrator on 25-4-17.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#endif //COMMUNICATION_H
#include <stdint.h>
#include <string.h>

#define MAX_COMM_CHANNELS 100 // 假设有最多5个通信通道
#define BUFFER_SIZE 1024    // 每个通道的缓冲区大小

// 定义通信结构体
typedef struct {
    int uid; // 处理器ID
    int pid; // 过程ID
    uint8_t* buffer; // 用于存储接收到的数据的缓冲区
    uint32_t buffer_size; // 缓冲区大小
    uint32_t data_length; // 当前缓冲区中数据的实际长度
    uint8_t is_connected; // 连接状态标志
} comm;

// 预先分配的通信通道和缓冲区
static comm channels[MAX_COMM_CHANNELS];
static uint8_t buffers[MAX_COMM_CHANNELS][BUFFER_SIZE];


// 建立到 (uid, pid) 的通信连接
int comopen(int uid, int pid, int flag, comm** f) {
    for(int i = 0; i < MAX_COMM_CHANNELS; ++i) {
        if(channels[i].is_connected == 0) { // 找到未使用的通道
            channels[i].uid = uid;
            channels[i].pid = pid;
            channels[i].data_length = 0;
            channels[i].is_connected = 1;
            *f = &channels[i];
            return 0; // 成功
        }
    }
    return -1; // 无可用通道
}

// 关闭通信连接
int comclose(comm** f) {
    if (*f == NULL) return -1;
    (*f)->is_connected = 0; // 标记为未连接
    (*f)->data_length = 0;
    return 0;
}


int comrecv(comm** f, void* buf, int len, int* wlen) {
    if ((*f)->data_length >= len) {
        memcpy(buf, (*f)->buffer, len);
        (*f)->data_length -= len;
        memmove((*f)->buffer, (char*)(*f)->buffer + len, (*f)->data_length);
        *wlen = len;
        return 0;
    }
    return -1; // 数据不足
}

int comsend(comm** f, void* buf, int len) {
    // 实现发送逻辑
    // ...
    return len; // 假定所有数据都已成功发送
}


// 复制式接收（不从缓冲区删去数据）
int comcp(comm** f, void* buf, int len, int* wlen) {
    if ((*f)->data_length >= len) {
        memcpy(buf, (*f)->buffer, len);
        *wlen = len;
        return 0;
    }
    return -1;
}

// 从缓冲区删除数据
int comerase(comm** f, int len) {
    if ((*f)->data_length >= len) {
        (*f)->data_length -= len;
        memmove((*f)->buffer, (char*)(*f)->buffer + len, (*f)->data_length);
        return 0;
    }
    return -1;
}

// 检查未读取的数据量
int comavail(comm** f, int* len) {
    *len = (*f)->data_length;
    return 0;
}

// 检查连接状态
int comcheck(comm** f) {
    return (*f)->is_connected;
}
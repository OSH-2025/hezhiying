//
// Created by Administrator on 25-4-17.
//

#include "comm.h"

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
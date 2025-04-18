//
// Created by Administrator on 25-4-17.
//

#ifndef PROCESS_H
#define PROCESS_H

#endif //PROCESS_H
typedef struct{
  int pid;//进程pid
  int status;//进程状态
}procinf;

enum{
  suspend=0,
realtime=1,
normal=2,};
int yield();
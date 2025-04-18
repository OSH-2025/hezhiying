//
// Created by Administrator on 25-4-17.
//

#include "signal.h"
#define maxsiglist 100
#include "../sys/timer/timer.h"
#include "../sys/task/task.h"
#include "../processcontrol/process.h"
int siglist[maxsiglist]={0};

int signew(int flag, int* sid){//flag=1全局，flag=0局部
    if(flag==1){//创建全局信号，此处暂时不写
      }
    if(flag==0){
      //局部信号，此处省略
    }
return 0;
}

int sigclose(int sig){
  siglist[sig]=-1;
  return sig;
}

int sigset(int sig, int s){
  siglist[sig]=s;
  return sig;
}

int waitsig(int sig, int timeout){
  uint time=0;
  uint t1l=ticklow;
  uint t1h=tickhigh;
  while(1){
    if(ticklow-t1l>timeout && timeout!=-1){
      return -1;
    }
    if(sig==1){
      return ticklow-t1l;
    }
	yield();
  }
  return time;
}

int waitany(const int* sig, int nsig,int timeout){
  uint time=0;
  uint t1l=ticklow;
  uint t1h=tickhigh;
  while(1){
    if(ticklow-t1l>timeout && timeout!=-1){
      return -1;
    }
    for(int i=0;i<nsig;i++){
      if(siglist[sig[i]]==1){
        return ticklow-t1l;
      }
    }
    yield();
  }
}

int readsig(int sig){
  return siglist[sig];
}

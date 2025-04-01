# Meeting Minutes for March 28th

[中文](March28-meeting_cn.md) | [English](March28-meeting.md)

Members: Zhuo Cui, Yanzhen Guo, Jiahong Luo, Wenkang Wei

## Meeting Content

1. Planning the Project Timeline

    - Running 1 task on 1 processor, i.e., the traditional MCU mode, while setting up an automated testing platform (using CI for automatic deployment and testing)
    - Running 2 tasks on 2 processors, completing communication debugging
    - Running 2 tasks on 1 processor, implementing task scheduling, etc.
    - Running 4 tasks on 2 processors, conducting overall testing of steps 1-3
    - Running 2 tasks on 2 processors, sharing memory
    - Running N tasks on N processors, conducting overall testing of steps 1-5
    - Running more than N tasks on N processors, fulfilling all requirements

2. Setting Up the Experimental Environment

    Jiahong Luo set up a basic experimental environment for us.

3. Cross-compilation Toolchain

    Using ARM GCC ( `arm-none-eabi-gcc` ) and Makefile

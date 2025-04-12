# 鹤之影 (hezhiying) @USTC

[中文](README_cn.md) | [English](README.md)

![logo](media/pict/logo_test.png)

## Team Members

- [Weikang Wei](https://github.com/jianyingzhihe)
- [Jiahong Luo](https://github.com/ustcljh)
- [Zhuo Cui](https://github.com/crosaa)
- [Yanzhen Guo](https://github.com/EricGuoYanzhen)

---

<h3 align="center">OSH-2025-hezhiying @University of Science and Technology of China</h3>

This is the GitHub homepage for the "鹤之影" (hezhiying) project, developed by students at the University of Science and Technology of China (USTC) for the OSH-2025 course. The project focuses on exploring topics related to operating systems, particularly in the context of embedded systems and microcontrollers.

<div align="center" markdown="1">

[![GitHub repo forks](https://img.shields.io/github/forks/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Forks)](https://github.com/OSH-2025/hezhiying/network)&#160;
[![GitHub repo stars](https://img.shields.io/github/stars/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Stars)](https://github.com/OSH-2025/hezhiying/stargazers)&#160;
[![GitHub repo contributors](https://img.shields.io/github/contributors-anon/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Contributors)](https://github.com/OSH-2025/hezhiying/graphs/contributors)
[![GitHub repo watchers](https://img.shields.io/github/watchers/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Watchers)](https://github.com/OSH-2025/hezhiying/watchers)&#160;
[![GitHub repo size](https://img.shields.io/github/repo-size/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Repo%20Size)](https://github.com/OSH-2025/hezhiying/archive/refs/heads/main.zip)

</div>

---

## Project Objectives

The goal of this project is to **design a distributed system based on a cluster of microcontrollers, interconnected via high-speed buses, utilizing shared RAM and a control core to achieve multitasking and multiprocess control**.

---

## Project Progress

|            Project Stage            |   Date    | Progress | Specific Tasks |                           Completion Status                           | Appendix                            |
|:-----------------------------------:|:---------:| :------: | :------------: |:---------------------------------------------------------------------:|-------------------------------------|
|      Individual Topic Research      | 2025.3.16 | Personal Research | Zhuo Cui: Rewriting StarPU, a multi-core embedded OS for robots, in Rust; Yanzhen Guo: Lightweight customization and memory management optimization of Chromium kernel; Weikang Wei: Designing an OS for Internet of Things (IoT); Jiahong Luo: Designing an OS for scheduling multiple microcontrollers and achieving multitask processing |                                 Done                                  | [log1](./logs/March19-meeting.md)   |
|     Group Discussion on Topics      | 2025.3.19 | Internal Exchange | After discussion, we found that rewriting existing systems in Rust for standalone embedded systems on robots may not significantly improve performance and lacks novelty. Chromium's kernel is too large, making trimming or rewriting less effective. Combining IoT with fine-grained OS design to manage hardware resources precisely aligns well with resource scheduling on microcontrollers. We discussed MCU design aimed at running multiple programs on a single chip, then extending to N chips each running N programs, ultimately aiming to run more than N programs across N chips |                                 Done                                  | [log2](./logs/March19-meeting.md)   |
|       Determining Group Topic       | 2025.3.19 | Topic Selection | Planning the research roadmap |                                 Done                                  | [log3](./logs/March28-meeting.md)   |
| Setting Up Experimental Environment | 2025.3.28 | Environment Setup | Jiahong Luo set up a basic experimental environment for us |                                 Done                                  | [log4](./logs/March28-meeting.md)   |
|                           Confirmed a system design focused on optimizing memory management, task scheduling, and inter-chip communication.                        | 2025.4.2  | Tech details | The meeting decided on a system design based on STM3F103 and GT6 processors, focusing on optimizing memory management, task scheduling, and inter-chip communication methods. |                                 Done                                  | [log5](./logs/April2-meeting.md)    |
|All APIs of the First Version Confirmed| 2025.4.9  |API Confirmation and Code Writing|Each team member has selected ("claimed") a portion of the APIs to work on.|Discussions have been completed, but code writing is still in progress.| [log6](./logs/April9-meeting_en.md) |
---

## [Submit Issues](https://github.com/OSH-2025/hezhiying/issues/new)

## [Contact Us](#team-members)

Through [GitHub Issues](https://github.com/OSH-2025/hezhiying/issues/new) or [Email](#team-members)

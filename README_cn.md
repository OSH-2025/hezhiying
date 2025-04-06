# 鹤之影 (hezhiying) @USTC

[中文](README_cn.md) | [English](README.md)

![logo](/src/pict/logo_test.png)

## 项目成员

- [位文康](https://github.com/jianyingzhihe)
- [罗嘉宏](https://github.com/ustcljh)
- [崔卓](https://github.com/crosaa)
- [郭彦禛](https://github.com/EricGuoYanzhen)

---

<h3 align="center">OSH-2025-鹤之影 @中国科学技术大学</h3>

这是“鹤之影” (hezhiying)项目的GitHub主页，该项目由中国科学技术大学（USTC）的学生为 OSH-2025 课程开发。项目专注于探索与操作系统相关的主题，特别是在嵌入式系统和单片机的背景下。

<div align="center" markdown="1">

[![GitHub repo forks](https://img.shields.io/github/forks/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Forks)](https://github.com/OSH-2025/hezhiying/network)&#160;
[![GitHub repo stars](https://img.shields.io/github/stars/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Stars)](https://github.com/OSH-2025/hezhiying/stargazers)&#160;
[![GitHub repo contributors](https://img.shields.io/github/contributors-anon/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Contributors)](https://github.com/OSH-2025/hezhiying/graphs/contributors)
[![GitHub repo watchers](https://img.shields.io/github/watchers/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Watchers)](https://github.com/OSH-2025/hezhiying/watchers)&#160;
[![GitHub repo size](https://img.shields.io/github/repo-size/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Repo%20Size)](https://github.com/OSH-2025/hezhiying/archive/refs/heads/main.zip)

</div>

---

## 项目目标

本项目旨在：**设计一个基于单片机集群的分布式系统，通过高速总线连接多个单片机，并利用共享 RAM 和一个控制核心来实现多任务处理和多进程控制**。

---

## 项目进度

| 项目阶段 | 日期 | 项目进展 | 具体分工 | 完成情况 | 附录 |
| :-----: | :---: | :-----: | :-----: | :------: | --- |
| 个人选题调研 | 2025.3.16 | 个人调研 | 崔卓：机器人上的多核嵌入式操作系统 StarPU 的 Rust 复写；郭彦禛：Chromium 内核轻量化定制与内存管理优化；位文康：物联网 IoT 的操作系统设计；罗嘉宏：多个单片机调度、实现多任务处理的操作系统设计 | 已完成 | [log1](/logs/March19-meeting_cn.md) |
| 个人选题交流 | 2025.3.19 | 组内交流 | 经过讨论，我们认为对于机器人上的独立嵌入式系统，用 Rust 复写并不利于系统性能的提升，而且 Rust 对于已有操作系统的复写缺乏新颖性。Chromium 内核过于庞大，进行裁剪或重写的效果可能不够理想。而结合 IoT 的细粒度操作系统设计，实现一个能够精细管理硬件资源的操作系统，这点与单片机上的资源调度有较高的契合性。我们讨论了 MCU 的设计方案，旨在实现在单一芯片上运行多个程序的能力，然后扩展到 N 个芯片分别运行 N 个程序，最终达到 N 个芯片运行超过 N 个程序的目标 | 已完成 | [log2](/logs/March19-meeting_cn.md) |
| 确定小组选题 | 2025.3.19 | 确定选题 | 规划了课题路线 | 已完成 | [log3](/logs/March28-meeting_cn.md) |
| 实验环境搭建 | 2025.3.28 | 环境搭建 | 罗嘉宏同学为我们搭建了一个基本的实验环境 | 已完成 | [log4](/logs/March28-meeting_cn.md) |

---

## [提交问题](https://github.com/OSH-2025/hezhiying/issues/new)

## [联系我们](#项目成员)

通过 [GitHub Issues](https://github.com/OSH-2025/hezhiying/issues/new) 或 [邮件](#项目成员)

# Meeting Minutes for March 19th

[中文](March19-meeting_cn.md) | [English](March19-meeting.md)

Members: Zhuo Cui, Yanzhen Guo, Jiahong Luo, Wenkang Wei

## Meeting Content

1. Discussion on Individual Research Topics

   - Zhuo Cui: Rewriting the multi-core embedded operating system StarPU in Rust for robots
   - Yanzhen Guo: Lightweight customization and memory management optimization of the Chromium kernel
   - Wenkang Wei: Designing an operating system for Internet of Things (IoT)
   - Jiahong Luo: Designing an operating system to schedule multiple microcontrollers and implement multitasking

2. Feasibility Analysis

   After discussion, we believe that rewriting an independent embedded system for robots using Rust does not significantly improve system performance and lacks novelty compared to existing operating systems. The Chromium kernel is too large, and its pruning or rewriting may not yield ideal results.

   In contrast, designing a fine-grained operating system in conjunction with IoT to achieve precise hardware resource management aligns well with resource scheduling on microcontrollers. We discussed the design of MCU schemes aimed at running multiple programs on a single chip, then expanding to N chips each running N programs, ultimately achieving more than N programs running across N chips.

3. Preliminary Selection of Group Topic

   Implementing "true" multitasking on a cluster of MCUs

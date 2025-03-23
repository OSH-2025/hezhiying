# 鹤之影 (hezhiying) @USTC  
[中文](README_zh.md) [英文](README_en.md)
<h3 align="center">OSH-2025-hezhiying @USTC</h3>

This is the GitHub homepage for the "鹤之影" (hezhiying) project, developed by students from the University of Science and Technology of China (USTC) for the OSH-2025 course. The project focuses on exploring topics related to operating systems, particularly in the context of embedded systems and single-chip microcomputers.

---

### 📦 version 
**v0.1.1** | Updated: 2025-03-23

---

## Team Members

- [jianyingzhihe](https://github.com/jianyingzhihe)  
- [罗嘉宏](https://github.com/ustcljh)  
- [崔卓](https://github.com/crosaa)  
- [郭彦禛](https://github.com/EricGuoYanzhen)  

---

## Project Progress

| Stage         | Task Details                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       | Date     | Progress | Notes |
|---------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------|----------|-------|
| Topic Selection | We had in-depth discussions around the topic of the big assignment. Firstly, we discussed the selection of underlying related content, such as multiprocessor systems or file systems, and mentioned the recently completed Linux 101 project. We believed that it could be expanded on this basis, but some people prefer the direction of combining with AI. However, after discussion, we believe that AI belongs to the application layer and is not closely related to the operating system, and we are concerned about insufficient innovation. Additionally, we explored the possibility of integrating the project with the Internet of Things (IoT), particularly focusing on resource scheduling and processing for multiple modules under a small memory footprint. This approach not only addresses performance issues on resource-constrained devices but also enhances system stability and efficiency, which is crucial for IoT devices.We have continued to consider previous year's titles and repo resources, but feel that simply copying them lacks novelty. We also proposed the embedded direction, believing that it can be combined with previously learned knowledge and has a stronger relevance to operating system courses. In order to better advance the project, we have decided to conduct research on the topics discussed today and will hold another meeting on Wednesday to discuss the feasibility of each direction and their specific implementation plans. | March 16 | Done     |       |
| Topic Selection | We delved into the details of Internet of Things (IoT) fine-grained operating system design, focusing on optimizing operating system functionalities in resource-constrained environments to support efficient and stable operations. IoT devices often face limitations in processing power, memory space, and energy supply. Therefore, our design goal is to create an operating system capable of meticulously managing hardware resources, ensuring multitasking and data transmission can be achieved even under extreme conditions.Regarding the enhancement of StarPU's functionality, we considered using C/C++ to improve performance and compatibility instead of choosing the Rust language. While Rust offers better security guarantees, C/C++ is more suitable for certain specific hardware interfaces and legacy system compatibilities. Especially when direct hardware manipulation or higher execution efficiency is required, the advantages of C/C++ become more pronounced.Furthermore, we discussed the design of microcontroller units (MCUs), aiming to enable the capability of running multiple programs on a single chip, then scaling up to N chips each running N programs, and ultimately achieving a scenario where N chips run more than N programs. To further advance these concepts, we plan to conduct in-depth research on related technical details and will discuss the specific implementation strategies and technical challenges of each direction in the upcoming meetings. This includes, but is not limited to, evaluating the impact of different programming languages on project goals, exploring new algorithms suitable for IoT devices, and testing system performance across various hardware configurations.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   | March 19 | Done     |       |
| Preliminary Topic Selection|Process Scheduling on Microcontrollers in Embedded Programming|Month3 23|Done
---

## Project Objectives

The project aims to:  
- Explore advanced operating system concepts in embedded systems.  
- Develop a practical solution for single-chip microcomputer applications.  
- Enhance skills in system design and implementation.  

---
## project todo
we shoud do:
- Research Existing Solutions:
  - Investigate existing embedded operating systems (such as FreeRTOS, Zephyr, etc.), understand how they manage multitasking scheduling, and evaluate their performance in resource-constrained environments.
  - Analyze whether these systems can be directly integrated or used as reference frameworks to accelerate our development process.
- Select Suitable Hardware Platforms:
    - Choose appropriate MCU models based on project requirements. Considering the balance between performance and cost, it may be necessary to test several different MCUs to find the optimal solution.
    - Ensure that the selected hardware supports necessary peripheral interfaces (such as SPI, I2C, etc.) for future expansion and interconnection.
- Design Efficient Scheduling Algorithms:
    - Develop scheduling algorithms suitable for resource-limited environments. Consider methods such as priority scheduling and round-robin scheduling, adjusting strategies according to actual application scenarios.
    - Implement load balancing techniques to ensure all participating MCUs can efficiently utilize their resources.
- Optimize Memory Management:
  - Design compact data structures and algorithms for small memory environments to reduce memory usage.
  - Explore the use of dynamic memory allocation combined with static memory partitioning to improve memory efficiency.
- Enhance Security and Reliability:
  - Consider security and reliability issues during the design phase, such as preventing deadlocks and handling exception situations.
  - Conduct unit tests and integration tests on critical code segments to ensure system stability and robustness.

---
## some references

---
## Control Options  

- **[Expand Progress Table](#project-progress)** - View project progress details  
- **[Jump to the top](#鹤之影-hezhiying-ustc)** - Jump to the top 
- **[View source code](https://github.com/hezhiying-ustc/OSH-2025-hezhiying/tree/main/src)** - View source code 
- **[Submit Issues](https://github.com/hezhiying-ustc/OSH-2025-hezhiying/issues/new)** - Report bugs or suggestions 

---

## Group Logo

![Group Logo](./src/fufu.jpg)  
*永世传颂，不休独舞，神爱世人，芙门永存！*  

---

## 附加信息 (Additional Info)  

- **Current version **: v0.1.1 
- **Update Log**:  
  - v0.1.0 (2025-03-22): Initialize the project homepage, add team information and preliminary progress.
  - v0.1.1 (2025-03-23): Preliminary project objectives were determined, and the project requirements and key factors were analyzed in detail
- **contact us**:  GitHub Issues or email ([click here and jump to us](#team-members)) 

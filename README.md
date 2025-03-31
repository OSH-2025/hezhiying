# 鹤之影 (hezhiying) @USTC  
[中文](README_cn.md) [英文](README.md)

---

## Team Members

- [jianyingzhihe](https://github.com/jianyingzhihe)  
- [罗嘉宏](https://github.com/ustcljh)  
- [崔卓](https://github.com/crosaa)  
- [郭彦禛](https://github.com/EricGuoYanzhen)  

<h3 align="center">OSH-2025-hezhiying @USTC</h3>

This is the GitHub homepage for the "鹤之影" (hezhiying) project, developed by students from the University of Science and Technology of China (USTC) for the OSH-2025 course. The project focuses on exploring topics related to operating systems, particularly in the context of embedded systems and single-chip microcomputers.

<div align="center" markdown="1">

[![GitHub repo forks](https://img.shields.io/github/forks/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Forks)](https://github.com/OSH-2025/hezhiying/network)&#160;
[![GitHub repo stars](https://img.shields.io/github/stars/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Stars)](https://github.com/OSH-2025/hezhiying/stargazers)&#160;
[![GitHub repo contributors](https://img.shields.io/github/contributors-anon/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Contributors)](https://github.com/OSH-2025/hezhiying/graphs/contributors)    
[![GitHub repo watchers](https://img.shields.io/github/watchers/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Watchers)](https://github.com/OSH-2025/hezhiying/watchers)&#160;
[![GitHub repo size](https://img.shields.io/github/repo-size/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Repo%20Size)](https://github.com/OSH-2025/hezhiying/archive/refs/heads/main.zip)

</div>

---

### 📦 version 
**v0.1.1** | Updated: 2025-03-23

---

## Project Progress

| Stage         | Task Details                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        | Date     | Progress | Notes |
|---------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------|----------|-------|
| Topic Selection | We had in-depth discussions around the topic of the big assignment. Firstly, we discussed the selection of underlying related content, such as multiprocessor systems or file systems, and mentioned the recently completed Linux 101 project. We believed that it could be expanded on this basis, but some people prefer the direction of combining with AI. However, after discussion, we believe that AI belongs to the application layer and is not closely related to the operating system, and we are concerned about insufficient innovation. Additionally, we explored the possibility of integrating the project with the Internet of Things (IoT), particularly focusing on resource scheduling and processing for multiple modules under a small memory footprint. This approach not only addresses performance issues on resource-constrained devices but also enhances system stability and efficiency, which is crucial for IoT devices.We have continued to consider previous year's titles and repo resources, but feel that simply copying them lacks novelty. We also proposed the embedded direction, believing that it can be combined with previously learned knowledge and has a stronger relevance to operating system courses. In order to better advance the project, we have decided to conduct research on the topics discussed today and will hold another meeting on Wednesday to discuss the feasibility of each direction and their specific implementation plans.                                                                                                                                                                                                                                                                  | March 16 | Done     |       |
| Topic Selection | We delved into the details of Internet of Things (IoT) fine-grained operating system design, focusing on optimizing operating system functionalities in resource-constrained environments to support efficient and stable operations. IoT devices often face limitations in processing power, memory space, and energy supply. Therefore, our design goal is to create an operating system capable of meticulously managing hardware resources, ensuring multitasking and data transmission can be achieved even under extreme conditions.Regarding the enhancement of StarPU's functionality, we considered using C/C++ to improve performance and compatibility instead of choosing the Rust language. While Rust offers better security guarantees, C/C++ is more suitable for certain specific hardware interfaces and legacy system compatibilities. Especially when direct hardware manipulation or higher execution efficiency is required, the advantages of C/C++ become more pronounced.Furthermore, we discussed the design of microcontroller units (MCUs), aiming to enable the capability of running multiple programs on a single chip, then scaling up to N chips each running N programs, and ultimately achieving a scenario where N chips run more than N programs. To further advance these concepts, we plan to conduct in-depth research on related technical details and will discuss the specific implementation strategies and technical challenges of each direction in the upcoming meetings. This includes, but is not limited to, evaluating the impact of different programming languages on project goals, exploring new algorithms suitable for IoT devices, and testing system performance across various hardware configurations. | March 19 | Done     |       |
| Preliminary Topic Selection| Process Scheduling on muti-Microcontrollers in Embedded Programming                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |March 23|Done||

eg. Subsequent working logs will be migrated to logs/.

---

## The project aims to:

### Design a distributed system based on a cluster of microcontrollers, connected through a high-speed bus. Utilize shared RAM and a control core to achieve multitasking and multiprocess control.
A schematic diagram is as follows:
```mermaid
graph TD
    subgraph 主控单元 [Control Unit]
        A1[Microcontroller A ]
        A2[External RAM]
        A1 --> A2
    end

    subgraph 块1 [Block 1]
        B1[Microcontroller B1]
        B2[Microcontroller B2]
        B3[Microcontroller B3]
        B4[Shared RAM B]
        B1 -.-> B4
        B2 -.-> B4
        B3 -.-> B4
    end

    subgraph 块2 [Block 2]
        C1[Microcontroller C1]
        C2[Microcontroller C2]
        C3[Microcontroller C3]
        C4[Shared RAM C]
        C1 -.-> C4
        C2 -.-> C4
        C3 -.-> C4
    end

    A1 -->|Task Allocation| B1
    A1 -->|Task Allocation| B2
    A1 -->|Task Allocation| B3
    A1 -->|Task Allocation| C1
    A1 -->|Task Allocation| C2
    A1 -->|Task Allocation| C3

    B1 -->|High-Speed Bus| B2
    B2 -->|High-Speed Bus| B3
    C1 -->|High-Speed Bus| C2
    C2 -->|High-Speed Bus| C3

    A1 -->|High-Speed Bus| B1
    A1 -->|High-Speed Bus| C1

    style A1 fill:#f96,stroke:#333,stroke-width:4px
    style A2 fill:#ddf,stroke:#333,stroke-width:2px
    style B4 fill:#ddf,stroke:#333,stroke-width:2px
    style C4 fill:#ddf,stroke:#333,stroke-width:2px
```

---
## Control Options  

- **[Expand Progress Table](#project-progress)** - View project progress details  
- **[Jump to the top](#鹤之影-hezhiying-ustc)** - Jump to the top 
- **[View source code](https://github.com/hezhiying-ustc/OSH-2025-hezhiying/tree/main/src)** - View source code 
- **[Submit Issues](https://github.com/hezhiying-ustc/OSH-2025-hezhiying/issues/new)** - Report bugs or suggestions 

---

## Group Logo

![Group Logo](src/pict/fufu.jpg)  
*永世传颂，不休独舞，神爱世人，芙门永存！*  

---

## Additional Info

- **Current version**: v0.1.1 
- **Update Log**:  
  - v0.1.0 (2025-03-22): Initialize the project homepage, add team information and preliminary progress.
  - v0.1.1 (2025-03-23): Preliminary project objectives were determined, and the project requirements and key factors were analyzed in detail
- **contact us**:  GitHub Issues or email ([click here and jump to us](#team-members)) 

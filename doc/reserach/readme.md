#  Research Report on Multi-MCU and Multi-Process 

---

## 1.  Research Background 
   An operating system is a core component of a computer system, responsible for managing hardware resources, coordinating task execution, and providing an interface between users and hardware. In traditional desktop and server environments, operating system design has become relatively mature. However, in the field of embedded systems, especially in resource-constrained Internet of Things (IoT) and MCU cluster scenarios, the design of operating systems still faces many challenges. With the popularization of IoT devices, the demand for low-power, small-memory, and high-reliability systems is increasing, driving the rapid development of research into embedded operating systems.

   Currently, embedded systems are widely used in smart homes, industrial automation, medical equipment, and other fields. The distributed design of MCU clusters can break through the performance bottlenecks of individual chips, enhancing the overall computing power and stability of the system. Therefore, the research of this project is not only a practical extension of the knowledge from the operating system course but also aligns with the trend of embedded technology development.

---
## 2.  Project Justification 

### Course Requirements and Knowledge Continuity : 

The OSH course aims to help us master the core principles of operating systems. Our team closely integrates these characteristics with those of embedded systems when selecting topics. The topic of multi-process scheduling in MCU clusters is highly relevant to course content (such as process management, memory allocation, communication mechanisms), which can effectively consolidate theoretical knowledge.

### Idea Source : 
 
During discussions on robot control, we realized there's a need for technology to synchronize the implementation of multiple processes instead of running them sequentially or synchronously. Current robot operating systems are single-core; our goal is to design a multi-core system to improve efficiency.

### Technical Challenges and Innovativeness : 

Compared to traditional MCU single-task execution, cluster design introduces complexity in multi-process scheduling and resource sharing.

### Practical Needs of IoT Applications : 

IoT devices usually operate in resource-constrained environments (such as small memory, low power consumption), requiring support from the operating system for efficient multitasking and data exchange. Through the design of MCU clusters and shared RAM, we explore the implementation of distributed systems on small-scale hardware, providing new ideas for optimizing the performance of IoT devices.

---

## 3.  Theoretical Knowledge 

 ### Processes and Scheduling : 

Process Management includes process states (ready, running, blocked), context switching, inter-process communication (IPC). Scheduling Algorithms include priority scheduling, round-robin scheduling, multilevel feedback queues, used for task allocation and load balancing. Multitasking involves implementing parallel running processes on resource-constrained MCUs.

 ### Memory Management : 

Shared Memory involves data sharing through external RAM, including memory allocation tables and access control (e.g., mutex locks). Memory Arbitration involves competition management of shared RAM between master and slave MCUs. Lightweight Design focuses on optimizing memory usage efficiency in small-memory environments.

 ### Communication Mechanisms : 

High-speed Bus protocols such as SPI, I2C, CAN, etc., their basic principles, and performance features. Interrupt Mechanism utilizes hardware interrupts to improve communication efficiency, avoiding resource wastage through polling. Protocol Design involves customizing communication protocols for task distribution and status feedback.

 ### Embedded System Design : 

Hardware Interfaces involve connecting MCUs with external RAM and buses. Distributed Architecture involves the implementation principles of master-slave models and cluster designs.

---

## 4.  Experimental Purpose

### Explore Distributed System Design: 

Implement task allocation and data sharing in MCU clusters, exploring methods for implementing distributed systems on small-scale hardware, providing references for IoT devices.

### Optimize System Performance: 

Compare the effects of different scheduling strategies and communication protocols through experiments, optimizing system efficiency and stability in resource-constrained environments.

---

## 5.  Operating Environment 
   According to project requirements, the operating environment is divided into hardware and software parts:
 ### Hardware Selection : 

Recommends using MCUs that support high-speed communication interfaces, such as STM32 series (supporting CAN, SPI, I2C, etc.), ESP32 (supporting Wi-Fi and multicore), or NXP’s i.MX RT series. It suggests choosing models with MMU or at least supporting RTOS for the main control MCU.

 ### System Segmentation : 

Divides the MCU group into several "blocks", each block achieves data exchange through shared RAM, with the main control MCU responsible for cross-block task scheduling.
 ### Task Allocation and Multitasking Processing : 
The main control MCU runs a real-time operating system (RTOS like FreeRTOS or uC/OS-III) responsible for task scheduling, resource management, and communication protocols. Slave MCUs run lightweight RTOS or bare metal code, receiving tasks assigned by the main control and executing them.

---

## 6.  Implementation Steps 
 ### Hardware Connection : 

A PCB board was designed to connect all MCUs to the board, significantly increasing communication speed compared to traditional DuPont wires.
 ### Software Design : 

The main control end initializes RTOS, creates a task scheduler, implements bus communication drivers (like SPI driver), and designs task allocation algorithms. The slave end initializes communication interfaces, listens to commands from the main control, executes assigned tasks, and returns results via shared RAM.

---

## 7.  Importance Analysis 
 ### Academic Value : 

This project focuses on multi-process scheduling across multiple chips rather than multi-core within a single chip, aiming to implement a simple library structure on STM32 for easy invocation of multi-process task interfaces.
 ### Technical Significance : 

The design of MCU clusters breaks through the performance limitations of single chips, enhancing system capabilities through hardware collaboration and software optimization. This method holds potential application value in IoT and edge computing fields.

---

## 8.  Related Work Investigation
### Classic Embedded Operating Systems: 
FreeRTOS, Zephyr 
### MCU Cluster Research:
StarPU, MPICH on MCU 
### Previous Project Cases:
OSH-2019, VR-fancy-office (OSH-2022)

---

## 9.  Innovative Points of This Project 
   Unlike previous projects focusing on multi-task processing within single or multi-core chips, this project proposes a cluster design, achieving distributed multi-process scheduling across different chips through high-speed buses and shared RAM, optimized for IoT scenarios while considering performance and resource constraints.
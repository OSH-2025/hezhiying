## April 2nd Meeting Summary

for more details click [here](./April2-meeting（detail）.md) and [HERE](4.2会议录音)

#### System Optimization and Program Execution
The meeting discussed system design using STM3F103 and GT6 processors, with the processor having one megabyte of flash and 96K RAM. It was decided to allocate 96K for system use and 64K for user programs. Both system and user programs are managed equally by the same scheduler due to the lack of built-in memory protection. User programs can be stored in flash or run directly from memory.

#### Memory Management and Task Scheduling
Tasks consist of code, data, and context, with data potentially stored in either flash or memory. The system can switch tasks at any time or automatically reclaim idle threads via a timer. Threads have priorities, and higher priority threads execute first. Four thread states were identified: suspended, sleeping, normally scheduled, and real-time mode for critical timing tasks.

#### Thread Management and Memory Swapping
Focus was on swapping threads in and out of memory, utilizing external storage, and direct memory address mapping through FSMC. A strategy was proposed to swap out threads that haven't been scheduled after ten attempts. Virtual memory is implemented using stacks to prevent global variable creation by threads, ensuring each thread only accesses its own memory area. Shared variables are managed through APIs provided by the system.

#### Thread Scheduling and Memory Area Management
Discussion included setting up areas for thread execution, noting that local variables contain addresses, thus different threads cannot share the same area. Three scheduling models were explored: independent scheduling per area, shared space among all threads, and priority-based scheduling across different areas. Preemptive and non-preemptive scheduling methods were also considered.

#### Memory Management and Pointer Operation Optimization
A fourth mode was introduced where the system automatically allocates memory and adjusts pointers, though this may slow down memory access speeds. This method involves intermediate mapping processes that could reduce overall efficiency during high-frequency operations like copying data.

#### Multitasking Switching in Hardware Mode
Discussions covered hardware usage, maintaining structures within the system's 64K space, and employing external memory for users. SMC characteristics enable multitasking switching by exposing itself only to the chip and accessing only 16 address lines. Strategies for optimizing OS resource management and communication efficiency were also addressed.

#### System Scheduling and Control Rights Transfer
System calls depend on millisecond interrupts, allowing the system to process tasks quickly before returning control to the user. The system gains control through terminal systems, special interrupts, or explicit user function calls. A watchdog timer mechanism ensures timely task handling.

#### Program Execution and State Tracking
Issues around thread switching and function calls were discussed, including storing and restoring the execution position of threads. Using program counters (PC) as a means of saving execution positions during interrupt handling was suggested.

#### Handling Thread Returns and Memory Management
When threads return, the system cleans up resources, including memory release. Users can request specific cleanup actions post-thread completion. Low-priority threads can clean unused memory areas when no other threads need scheduling.

#### Internal/External Memory Management and Global Variable Usage
The implementation of internal and external memory was discussed, along with the risks associated with using global variables, recommending against their use to avoid data inconsistency issues.

#### System Synchronization and Multi-thread Management
Single-core and multi-core working modes, synchronization locks, semaphores, and resource protection strategies using the interrupt system for peripherals were discussed. The importance of network concepts in simulations and establishing connections between chips was highlighted.

#### Inter-chip Communication and Single Chip Optimization
Communication between multiple chips and optimization strategies for single-chip designs were explored. Despite current limitations, improvements were left open for future developments, focusing on normal inter-chip communication and optimizing designs to reduce signal lines and efficiently utilize exclusive memory on a single chip. Serial communication and shared memory are key for large data storage.
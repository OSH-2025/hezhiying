# 鹤之影 (hezhiying) @USTC
[中文](README_cn.md) [英文](README.md)

## 团队成员

- [位文康](https://github.com/jianyingzhihe)
- [罗嘉宏](https://github.com/ustcljh)
- [崔卓](https://github.com/crosaa)
- [郭彦禛](https://github.com/EricGuoYanzhen)

------

<h3 align="center">OSH-2025-鹤之影 @中国科学技术大学</h3>

这是“鹤之影”(hezhiying)项目的GitHub主页，该项目由中国科学技术大学（USTC）的学生为OSH-2025课程开发。项目专注于探索与操作系统相关的主题，特别是在嵌入式系统和单片机的背景下。

<div align="center" markdown="1">

[![GitHub repo forks](https://img.shields.io/github/forks/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Forks)](https://github.com/OSH-2025/hezhiying/network)&#160;
[![GitHub repo stars](https://img.shields.io/github/stars/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Stars)](https://github.com/OSH-2025/hezhiying/stargazers)&#160;
[![GitHub repo contributors](https://img.shields.io/github/contributors-anon/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Contributors)](https://github.com/OSH-2025/hezhiying/graphs/contributors)    
[![GitHub repo watchers](https://img.shields.io/github/watchers/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Watchers)](https://github.com/OSH-2025/hezhiying/watchers)&#160;
[![GitHub repo size](https://img.shields.io/github/repo-size/OSH-2025/hezhiying?style=flat&logo=github&logoColor=whitesmoke&label=Repo%20Size)](https://github.com/OSH-2025/hezhiying/archive/refs/heads/main.zip)

</div>

---

### 📦 当前版本
**v0.1.1** | 更新时间: 2025-03-23

---

## 项目进度

| 阶段       | 任务详情                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         | 日期   | 完成情况 | 附录 |
|----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------|------|----|
| 课题选择     | 我们围绕大作业的主题进行了深入讨论。首先，我们讨论了底层相关内容的选择，比如多处理器系统或文件系统，并提到了最近完成的Linux 101项目。我们认为可以在该项目的基础上进行扩展，但有些人倾向于结合人工智能的方向。然而，在讨论之后，我们认为人工智能属于应用层，并与操作系统没有紧密的联系，同时我们也担心这方面的创新不足。此外，我们探讨了将项目与物联网（IoT）相结合的可能性，特别是关注在小内存环境下多个模块的资源调度和处理。这种方法不仅解决了资源受限设备上的性能问题，还增强了系统的稳定性和效率，这对物联网设备至关重要。我们继续考虑了往年的题目和仓库资源，但感觉单纯地复制它们缺乏新颖性。我们提出了嵌入式方向，认为它可以与之前学习的知识相结合，且与操作系统课程有更强的相关性。为了更好地推进项目，我们决定对今天讨论的主题进行研究，并定于周三再次开会详细讨论各个方向的可行性及其具体实施方案。                                                                                 | 3.16 | 完成   |    |
| 课题选择以及讨论 | 我们深入探讨了物联网（IoT）细粒度操作系统设计的细节，重点在于优化资源受限环境下的操作系统功能，以支持高效和稳定的运行。物联网设备通常面临处理能力、内存空间和能源供应的限制。因此，我们的设计目标是创建一个能够精细管理硬件资源的操作系统，确保即使在极端条件下也能实现多任务处理和数据传输。关于增强StarPU的功能性，我们考虑使用C/C++来提高性能和兼容性，而不是选择Rust语言。虽然Rust提供了更好的安全性保障，但在某些特定的硬件接口和遗留系统的兼容性方面，C/C++更为合适。特别是在需要直接操作硬件或追求更高执行效率时，C/C++的优势更加明显。此外，我们讨论了微控制器单元（MCU）的设计方案，旨在实现在单一芯片上运行多个程序的能力，然后扩展到N个芯片分别运行N个程序，最终达到N个芯片运行超过N个程序的目标。为了进一步推进这些概念，我们计划深入研究相关技术细节，并将在即将举行的会议中详细讨论每个方向的具体实施策略和技术挑战。这包括但不限于评估不同编程语言对项目目标的影响、探索适用于物联网设备的新算法以及测试各种硬件配置下的系统性能。 | 3.19 | 完成   |    |
| 初步确定选题   | 嵌入式编程中的单片机集群多进程调度                                                                                                                                                                                                                                                                                                                                                                                                                                                                            | 3.23 | 完成   |    |

此后工作日志迁移至./logs/ 目录下。

---

## 项目目标

项目旨在：
### 设计一个基于单片机集群的分布式系统，通过高速总线连接多个单片机，并利用共享RAM和一个控制核心来实现多任务处理和多进程控制。


---
## 控制选项

- **[展开进度表格](#project-progress)** - 查看项目进度详情
- **[跳转到顶部](#鹤之影-hezhiying-ustc)** - 返回页面顶部
- **[查看源代码](https://github.com/hezhiying-ustc/OSH-2025-hezhiying/tree/main/src)** - 浏览项目代码
- **[提交问题](https://github.com/hezhiying-ustc/OSH-2025-hezhiying/issues/new)** - 报告 bug 或建议

---

## 团队 Logo 

![Group Logo](src/pict/fufu.jpg)  
*永世传颂，不休独舞，神爱世人，芙门永存！*

---

## 附加信息

- **当前版本**: v0.1.1
- **更新日志**:
    - v0.1.0 (2025-03-22): 初始化项目主页，添加团队信息和初步进度。
    - v0.1.1(2025-03-23):初步确定项目目标，并详细分析了项目的需求以及关键
- **[联系我们](#团队成员)**: 通过 GitHub Issues 或邮件 ([点击这里找到我们](#团队成员))

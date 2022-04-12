2021-07-30
正在编写c++通用库

2021-09-29
- 研究CVDevelop的技术选型
- opengl 

2022-04-12
大幅度修改了 Common 基础库
- 重写了 System::List (线程不安全), 通过预定义 Item 的释放策略 可实现 RALL 的 std::vector.(玩具版)
- 重写了 System::IO::File/FileStream
- 重写了 System::Socket 提供Windows 下最先进的异步 Socket 架构。 (ThreadPool + IOCP)
- 重写了 System::Threading::Thread
- 新增了线程同步对象
- 新增了线程池( windows wrapper )
- 新增了 System::Threading::Tasks::Task
```
 Tips:
    Task 借鉴了 .net 的 Task 实现机制( ThreadPool + IOCP)
```
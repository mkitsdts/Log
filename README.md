# C++低性能单头文件跨平台日志库

纯乐子。

日志分为四个等级，分别是：

* DEBUG

* INFO

* WARNING

* ERROR

已经实现:

* 日志内容包含等级和输出信息

* 异步输出日志

* 日志内容时间

保证线程安全的方法：

* 在消息队列达到上限时上锁

已知bug：主线程直接退出会导致日志无法完全输出（一定程度上已修复）。

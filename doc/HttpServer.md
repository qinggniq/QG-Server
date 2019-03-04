# Web Server 设计草稿

## 动机
决定用C++做一个*Web Server*，来验证自己所学的关于*C++, Internet Programing, Desing Pattern*的知识。草稿，顾名思义就是用来记录自己所见所查所思。

## 类似项目篇

### cinatra
[cinatra](https://github.com/qicosmos/cinatra)是一个高性能易用的http框架，它是用modern c++(c++17)开发的，它的目标是提供一个快速开发的c++ http框架。它的主要特点如下:  
1. 统一而简单的接口
2. header-only
3. 跨平台
4. 高效
5. 支持面向切面编程

支持http1.1/1.0，ssl和websocket。
#### 可借鉴的东西
1. ssl和websocket的支持
2. 面向切面编程

#### ps
对于我来说这些东西为时尚早。
### 


## 基础知识篇

### http/1.1报文格式


### TCP网络基础知识

### Linux Socket编程
#### 基本服务器处理流程
##### 服务器端
##### 客户端

#### 标准函数
##### socket()
##### bind()
##### listen()
##### accept()
##### connect()




### Linux进程IPC
### Linux多线程
#### 线程创建
#### 线程退出
#### race condition 线程同步
##### 互斥锁
##### 读写锁
##### 自旋锁

### 服务器并发请求处理架构
#### reactor模式
#### poactor模式
#### peer connection peer process 模型
#### peer connection peer thread 模型


### Linux IO
#### 读写缓冲
#### 阻塞读写
#### 非阻塞读写
#### 异步读写


### 基本数据结构
#### 小顶堆
#### 链表


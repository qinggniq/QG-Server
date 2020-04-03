# Problems
## 多线程
> 引入多线程Reactor之后监听connection的线程一直阻塞在loop那里

由于loop默认设置的是
```c++
if (has_timer) wait_time = lasted_elasped - now;
else wait_time = forever;
```
每个线程一个event loop，主线程Accpte新连接，然后分发到子线程。在子线程创建的时候开始loop，这个时候由于里面监听的fds是空，并且超时时间设置的永远。导致来了新连接之后loop子线程一直不返回。

好像这是由于poll()函数的调用方式的关系，poll的update仅仅update的用户空间的数据结构，然而poll()监测的是poll()函数调用那个时刻传进去的fd集合。

一开始fd集合为空，所以会一直阻塞在poll调用那里。

有三种解决方式：
1. 在每个loop里面加一个pipe fd，来一个新连接的时候就先update，然后再写pipe fd，可以唤醒那个loop。
2. 设置一个默认超时（比如1s），这样就永远不会一直loop了。
3. 换成epoll 

目前是第一个方法和第三个方法结合使用，如果`POLL`类型是`epoll`，那么什么都不做，如果是`poll`类型，那么使用第一个方法。

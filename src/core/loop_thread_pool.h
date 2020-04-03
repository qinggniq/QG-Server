//
// Created by 王聪 on 2020/4/3.
//

#ifndef QGSERVER_SRC_CORE_LOOP_THREAD_POOL_H_
#define QGSERVER_SRC_CORE_LOOP_THREAD_POOL_H_
#include "../util/noncopyable.h"
#include "loop_thread.h"
#include <vector>
namespace qg {
class EventLoop;
class LoopThread;
class LoopThreadPool : noncopyable {
public:
  LoopThreadPool(EventLoop *base_loop, int thread_size)
      : base_loop_(base_loop), thread_size_(thread_size < 0 ? 0 : thread_size), curPos(0){
  }
  void start();

  EventLoop* getNextLoop();

private:
  const int thread_size_;
  int curPos;
  EventLoop *base_loop_;
  std::vector<EventLoop *> loops_;
  std::vector<std::unique_ptr<LoopThread>> threads_;
};
} // namespace qg

#endif // QGSERVER_SRC_CORE_LOOP_THREAD_POOL_H_

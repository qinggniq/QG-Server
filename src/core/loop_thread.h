//
// Created by 王聪 on 2020/4/3.
//

#ifndef QGSERVER_SRC_CORE_LOOP_THREAD_H_
#define QGSERVER_SRC_CORE_LOOP_THREAD_H_

#include "../util/type.h"
#include <mutex>
#include <thread>
#include <utility>

namespace qg {
class EventLoop;
class LoopThread {
public:
  LoopThread() :thread_(std::bind(&LoopThread::threadFunc, this)){}
  LoopThread(qg_string name) : name(std::move(name)), thread_(std::bind(&LoopThread::threadFunc, this)) {}
  ~LoopThread() {
    thread_.join();
  }
  EventLoop *getLoop();

private:
  void threadFunc();
  qg_string name;
  EventLoop *loop_;
  std::mutex mutex_;
  std::condition_variable created_cond_;
  std::thread thread_;
};
} // namespace qg

#endif // QGSERVER_SRC_CORE_LOOP_THREAD_H_

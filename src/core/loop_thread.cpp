//
// Created by 王聪 on 2020/4/3.
//

#include "loop_thread.h"
#include "event_loop.h"

namespace qg {
EventLoop * LoopThread::getLoop() {
  std::unique_lock<std::mutex> ulock(mutex_);
  LOG(INFO) << "getLoop ";
  while (loop_ == nullptr) {
    created_cond_.wait(ulock);
  }
  LOG(INFO) << "over";
  return loop_;
}

void LoopThread::threadFunc() {
  LOG(INFO) << "thread created before";
  EventLoop loop;
  {
    std::unique_lock<std::mutex> ulock(mutex_);
    loop_ = &loop;
    created_cond_.notify_one();
  }
  LOG(INFO) << "thread created ";
  loop_->loop();
  std::unique_lock<std::mutex> ulock(mutex_);
  loop_ = nullptr;
}
}
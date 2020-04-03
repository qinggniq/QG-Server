//
// Created by 王聪 on 2020/4/3.
//

#include "loop_thread_pool.h"
#include "loop_thread.h"
#include <memory>
#include <glog/logging.h>

namespace qg {
void LoopThreadPool::start() {
  LOG(INFO) << "start loop thread pool";
  for (int i = 0; i < thread_size_; ++i) {
    qg_string name = "loop thread : " + std::to_string(i);
    threads_.push_back(std::make_unique<LoopThread>(name));
    loops_.push_back(threads_[i]->getLoop());
    LOG(INFO) << "thread : " << i << " added";
  }
}

EventLoop *LoopThreadPool::getNextLoop() {
  EventLoop *loop = base_loop_;
  if (thread_size_) {
    loop = loops_[curPos];
    curPos++;
    if (curPos >= thread_size_)
      curPos = 0;
  }
  return loop;
}
} // namespace qg
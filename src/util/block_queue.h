//
// Created by 王聪 on 2020/4/3.
//

#ifndef QGSERVER_SRC_UTIL_BLOCK_QUEUE_H_
#define QGSERVER_SRC_UTIL_BLOCK_QUEUE_H_
#include "noncopyable.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <glog/logging.h>
namespace qg {
template <typename T> class Queue  {
public:
  Queue() = default;
  ~Queue() {
    LOG(INFO) << "queue end" ;
  }
  void push(const T &t){
    std::unique_lock<std::mutex> ulock(mutex_);
    queue_.push(t);
    not_empty_.notify_one();
  }
  T pop() {
    std::unique_lock<std::mutex> ulock(mutex_);
    while (queue_.empty()) {
      not_empty_.wait(ulock);
    }
    T res = queue_.front();
    queue_.pop();
    return res;
  }
 void pop_all(std::queue<T>& res) {
    std::unique_lock<std::mutex> ulock(mutex_);
    LOG(INFO) << "lock after";
    if (queue_.empty()) return;
    std::swap(res, queue_);
  }

private:
  std::mutex mutex_;
  std::condition_variable not_empty_;
  std::queue<T> queue_;
};

} // namespace qg

#endif // QGSERVER_SRC_UTIL_BLOCK_QUEUE_H_

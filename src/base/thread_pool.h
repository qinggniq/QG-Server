//
// Created by wc on 5/13/19.
//

#ifndef SRC_THREAD_POOL_H
#define SRC_THREAD_POOL_H
#include "../util/noncopyable.h"
#include "../util/type.h"
#include <pthread.h>
#include <queue>

namespace qg {

template <typename T> class ThreadPool : public noncopyable {
public:
  ThreadPool(qg_size_t pool_size, qg_size_t queue_size);
  ~ThreadPool() = default;

  void addTask(T *t);
  void execTask();
  void start();

private:
  const qg_size_t max_req_size_;
  const qg_size_t pool_size_;
  std::queue<T *> task_queue_;
  pthread_mutex_t mutex_;
  pthread_cond_t no_full_;
  pthread_cond_t no_empty_;
};
} // namespace qg

#endif // SRC_THREAD_POOL_H

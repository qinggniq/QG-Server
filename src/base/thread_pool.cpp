//
// Created by wc on 5/13/19.
//

#include "thread_pool.h"

namespace qg{
template<typename T>
ThreadPool<T>::ThreadPool(qg_size_t pool_size, qg_size_t queue_size) : pool_size_(pool_size), max_req_size_(sz) {
  if (pthread_mutex_init(&mutex, nullptr) != 0) {
    //TODO log
  }
  if (pthread_cond_init(&no_empty_, nullptr) != 0) {
    //TODO log

  }
  if (pthread_cond_init(&no_full_, nullptr) != 0) {
    //TOOD log
  } 
}

template<typename T>
void ThreadPool<T>::addTask(T* t) {
  pthread_mutex_lock(&mutex_);
  while (task_queue_.size() >= max_req_size_) {
    pthread_cond_wait(&no_full_, &mutext_);
  }
  queue.push(t);
  pthread_cond_signal(&no_empty_);
  pthread_mutex_unlock(&mutex);
}
template<typename T>
void ThreadPool<T>::execTask() {
  while (true) {
    pthread_muex_lock(&mutex_);
    while (!task_queue_.empty()) {
      pthread_cond_wait(&no_empty_, &mutex_);
    }
    T* t = task_queue_.front();
    task_queue.pop();
    pthread_cond_signal(&no_full_);
    pthread_mutext_unlock(&mutex_);
    t();              // T是一个functor
  }
}
template<typename T>
void ThreadPool<T>::start() {
  pthread_t threads[pool_size_];
  for (int i=0; i<pool_size_; ++i) {
    pthread_create(&threads[i], nullptr, execTask, nullptr);
    pthread_detach(&threads[i]);
  }
}
}//namespace qg
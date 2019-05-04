//
// Created by wc on 5/3/19.
//

#include "timer_heap.h"
namespace qg{
namespace timer_tool{
qg_fd_t createTimerfd(){
  qg_fd_t timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
  if (timer_fd == -1) {
    //TODO log ("create Timer failed!")
  }
  return timer_fd;
}

void readTimerfd(qg_fd_t fd) {
  qg_uint64_t exp;
  qg_ssize_t s = read(fd, &exp, sizeof(qg_uint64_t));
  if (s != sizeof(qg_uint64_t)) {
    //TODO log ("read tiemrfd failed")
  }
}

void resetTiemrfd(qg_fd_t fd, TimeStamp new_time) {
  struct itimerspec new_value, old_value;
  struct timespec ts;
  memset(&new_value, 0, sizeof(struct itimerspec));
  memset(&old_value, 0, sizeof(struct itimerspec));
  ts.tv_nsec = new_time.getUnixTimeStamp() / TimeStamp::kMicroSecondsPerSecond;
  ts.tv_sec = new_time.getUnixTimeStamp() % TimeStamp::kMicroSecondsPerSecond * 1000;
  new_value.it_value = ts;
  if ((::timerfd_settime(fd, 0, &new_value, &old_value)) != 0) {
    //TODO log
  }
  //TODO fininsh the logic of reset timer;
}
}



void TimerHeap::addTimer(qg::TimerHeap::timer_pt &timer) {
  heap_.push_back(timer);
  up(heap_.size()-1);
}

void TimerHeap::popTimer() {
  if (heap_.empty()) return;
  //TimerHeap::swap(0, heap_.size()-1); //TODO find why not work
  heap_[0] = heap_[heap_.size()-1];
  down(0);
  heap_.pop_back();
}

void TimerHeap::up(qg::qg_size_t idx) {
  qg_size_t parent = (idx - 1)>>1;
  while (idx > 0 && *heap_[idx] < *heap_[parent]) {
    TimerHeap::swap(idx, parent);
    idx = parent;
    parent = (idx - 1)>>1;
  }
}

void TimerHeap::down(qg::qg_size_t idx) {
  qg_size_t child = (idx << 1) + 1;
  while (child < heap_.size()) {
    qg_size_t minChild = (child + 1 == heap_.size()
    					||*heap_[child] < *heap_[child+1]) ? child : child+1;
    if (*heap_[idx] < *heap_[minChild]) break;
    TimerHeap::swap(idx, minChild);
	idx = minChild;
	child = (idx << 1) + 1;
  }
}

void TimerHeap::swap(qg::qg_size_t upidx, qg::qg_size_t downidx) {
  //std::swap(*heap_[upidx], *heap_[downidx]);
  auto tmp = heap_[upidx];
  heap_[upidx] = heap_[downidx];
  heap_[downidx] = tmp;
}
} //namespace qg
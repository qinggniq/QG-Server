//
// Created by wc on 5/3/19.
//

#include "timer_heap.h"
namespace qg{

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
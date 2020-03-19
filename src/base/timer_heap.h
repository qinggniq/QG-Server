//
// Created by wc on 5/3/19.
//

#ifndef SRC_TIMER_HEAP_H
#define SRC_TIMER_HEAP_H

#include <vector>

#include "../util/time_stamp.h"
#include "../util/type.h"
#include "timer.h"

namespace qg {

class Timer;
class TimerHeap {
public:
  typedef Timer *timer_pt;
  void addTimer(timer_pt timer);
  void popTimer();
  // No need to cancel the timer
  // void detachTimer(qg_size_t idx);
  timer_pt top() { return heap_.empty() ? nullptr : heap_[0]; }
  qg_bool empty() { return heap_.empty(); }

private:
  void down(qg_size_t idx);
  void up(qg_size_t idx);
  void swap(qg_size_t upidx, qg_size_t downidx);

private:
  std::vector<timer_pt> heap_;
};

} // namespace qg

#endif // SRC_TIMER_HEAP_H

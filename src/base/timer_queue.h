//
// Created by wc on 5/4/19.
//

#ifndef SRC_TIMER_QUEUE_H
#define SRC_TIMER_QUEUE_H

#include "../util/noncopyable.h"
#include "../util/type.h"

namespace qg {

class EventLoop;
class EventHandler;
class TimerHeap;
class Timer;
class TimerQueue : public noncopyable {
public:
  typedef EventLoop event_loop_pt;
  typedef EventHandler *event_handler_pt;
  typedef std::shared_ptr<Timer> timer_pt;
#ifdef __linux__
  explicit TimerQueue(event_loop_pt el, qg_fd_t);
#elif __APPLE__
  TimerQueue();
#endif
  ~TimerQueue();
  TimeStamp getNextTimeStamp() const { return next_time_; }
  void addTimer(timer_pt timer);
  void handleTimer();
  bool _push(timer_pt timer);

private:
  TimerHeap *timer_heap_;
  TimeStamp next_time_;
#ifdef __linux__
  qg_fd_t timer_fd_;
  event_handler_pt handler_;
  event_loop_pt event_loop_;
#endif
};
} // namespace qg

#endif // SRC_TIMER_QUEUE_H

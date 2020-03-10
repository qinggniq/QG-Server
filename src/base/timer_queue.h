//
// Created by wc on 5/4/19.
//

#ifndef SRC_TIMER_QUEUE_H
#define SRC_TIMER_QUEUE_H

#include "type.h"
#include "noncopyable.h"



namespace qg{

class EventLoop;
class EventHandler;
class TimerHeap;
class Timer;
class TimerQueue:public noncopyable{
 public:
  typedef EventLoop event_loop_pt;
  typedef EventHandler* event_handler_pt;
  typedef Timer* timer_pt;

  explicit TimerQueue(event_loop_pt el);
  ~TimerQueue();
  void addTimer(timer_pt timer);

  //TODO add the function of canceling timer
  //use HashMap <timerId, heapIdx>
  //void cancelTimer(timer_pt timer);
  void handleTimer();
  bool _push(timer_pt timer);

 private:
  qg_fd_t timer_fd_;

  TimerHeap timer_heap_;
  event_loop_pt loop_;
  //register in
  event_handler_pt handler_;
};
}


#endif //SRC_TIMER_QUEUE_H

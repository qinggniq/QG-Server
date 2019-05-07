//
// Created by wc on 5/4/19.
//

#ifndef SRC_TIMER_QUEUE_H
#define SRC_TIMER_QUEUE_H
#include "timer_heap.h"
#include "type.h"
#include "noncopyable.h"
#include "event_handler.h"
#include "reactor.h"

namespace qg{

class TimerQueue:public noncopyable{
 public:
  typedef std::shared_ptr<Dispatcher> event_loop_pt;
  typedef std::shared_ptr<EventHandler> event_handler_pt;
  typedef std::shared_ptr<Timer> timer_pt;

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

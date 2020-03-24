//
// Created by wc on 3/6/19.
//

#ifndef SRC_TIMER_H
#define SRC_TIMER_H

#include "../util/noncopyable.h"
#include "../util/time_stamp.h"
#include "../util/type.h"

namespace qg {

class TimerHeap;
class TimeStamp;
class Timer : public noncopyable {
public:
  Timer(TimeStamp when, qg_time_t interval, timer_handler_cb_t call_back,
        qg_bool repeat = false);

  TimeStamp expire() const { return expire_; }
  qg_bool repeat() const { return repeat_; }
  qg_bool timeout() const { return TimeStamp::Now() >= expire(); }

  void update(qg_time_t timeout);
  void run();
  void restart();

  bool operator<(const Timer &rhs) { return expire() < rhs.expire(); }

private:
  friend class TimerHeap;
  TimeStamp expire_;
  qg_bool repeat_;
  qg_time_t interval_;
  timer_handler_cb_t timer_handler_;
};

} // namespace qg
#endif // SRC_TIMER_H

//
// Created by wc on 3/6/19.
//

#include "timer.h"
#include <chrono>

namespace qg {


Timer::Timer(TimeStamp when,
      const timer_handler_cb_t& call_back,
      qg_time_t interval) :
      expire_(when),
      timer_hander_(std::move(call_back)),
      interval_(interval)
      {}



void Timer::update(qg_int timeout) {
  //auto time_point = std::chrono::system_clock::now();
  //TODO int -> double
  if (timeout < 100) timeout = 100;
  expire_ = addTime(expire_, timeout);//std::chrono::system_clock::to_time_t(time_point) + timeout;
}

void Timer::run() {
  timer_hander_();
}

TimeStamp Timer::now() {
  auto time_point = std::chrono::system_clock::now();
  auto res = std::chrono::system_clock::to_time_t(time_point);
  return TimeStamp(res);
}


}//namespace qg
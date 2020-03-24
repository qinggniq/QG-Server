//
// Created by wc on 3/6/19.
//

#include "timer.h"

#include <chrono>

namespace qg {

Timer::Timer(TimeStamp when, qg_time_t interval, timer_handler_cb_t call_back,
             qg_bool repeat)
    : expire_(when), timer_handler_(std::move(call_back)), interval_(interval),
      repeat_(repeat) {}

void Timer::update(qg_time_t timeout) {
  // 一个timeout代表一秒
  // std::chrono::system_clock::to_time_t(time_point) + timeout;
  expire_ = addTime(expire_, timeout);
}

void Timer::run() { timer_handler_(); }

void Timer::restart() { update(interval_); }

} // namespace qg
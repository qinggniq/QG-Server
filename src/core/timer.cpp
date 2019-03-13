//
// Created by wc on 3/6/19.
//

#include "timer.h"

namespace qg {
void Timer::Restart(qg::TimeStamp now) {
  if (repeat_) {
    expiration_ = addTime(now, interval_);
  } else {
    expiration_ = TimeStamp(0);
  }
}
}//namespace qg
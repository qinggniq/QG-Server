//
// Created by wc on 3/6/19.
//

#include "time_stamp.h"
#include <iomanip>
#include <chrono>

namespace qg{
TimeStamp::TimeStamp():unix_time_stamp_(0) {}

qg_string
TimeStamp::toString() const {
  qg_ostringstream os;
  os  << std::setfill('0') << std::setw(6) << " " << unix_time_stamp_ / kMicroSecondsPerSecond \
      << std::setfill('0') << std::setw(6) << " " << unix_time_stamp_ % kMicroSecondsPerSecond;
  return os.str();

}

qg_string
TimeStamp::toFormatedString() const {
  return qg_string();
}

TimeStamp TimeStamp::Now()
{
  auto t = std::chrono::system_clock::now();
  qg_int64_t time_t = std::chrono::system_clock::to_time_t(t);
  return TimeStamp(time_t);
}
}//namespace qg
//
// Created by wc on 3/6/19.
//

#include "time_stamp.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <sys/time.h>

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
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return TimeStamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}
}//namespace qg
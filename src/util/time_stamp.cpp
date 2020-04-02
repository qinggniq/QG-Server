//
// Created by wc on 3/6/19.
//

#include "time_stamp.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <sys/time.h>

namespace qg {
TimeStamp::TimeStamp() : unix_time_stamp_(0) {}

qg_string TimeStamp::toString() const {
  qg_ostringstream os;
  os << std::setfill('0') << std::setw(6) << " "
     << unix_time_stamp_ / kMicroSecondsPerSecond << std::setfill('0')
     << std::setw(6) << " " << unix_time_stamp_ % kMicroSecondsPerSecond;
  return os.str();
}

qg_string TimeStamp::toFormatedString() const {
  char buf[50];
  (::sprintf(buf, "unix timestamp : %lld", unix_time_stamp_));
  return qg_string(buf);
}

TimeStamp TimeStamp::Now() {
  auto time_point = std::chrono::system_clock::now();
  auto res = std::chrono::system_clock::to_time_t(time_point);
  return TimeStamp(res * TimeStamp::kMicroSecondsPerSecond);
}

qg_string TimeStamp::getTimeOfDay() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t nowtime = tv.tv_sec;
  struct tm tm_time;
  gmtime_r(&nowtime, &tm_time);
  char buf[128];
  size_t len = strftime(buf, 128, "%c %Z", &tm_time);
  return qg_string(buf, buf + len);
}
} // namespace qg
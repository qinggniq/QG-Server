//
// Created by wc on 3/6/19.
//

#ifndef SRC_TIME_STAMP_H
#define SRC_TIME_STAMP_H

#include "type.h"
#include "copyable.h"

namespace qg{
class Copyable;
class TimeStamp:public copyable {
 public:
  TimeStamp();
  explicit TimeStamp(const qg_int64_t unixTimeStamp):unix_time_stamp_(unixTimeStamp) {};
  qg_string toString() const;
  qg_string toFormatedString() const;

  qg_int64_t getUnixTimeStamp() const {return unix_time_stamp_;}

  static TimeStamp Now();

  static const qg_int kMicroSecondsPerSecond = 1000 * 100;
 private:
  qg_int64_t unix_time_stamp_;

};

inline
qg_bool
operator<(TimeStamp lhs, TimeStamp rhs) {
  return lhs.getUnixTimeStamp() < rhs.getUnixTimeStamp();
}

inline
qg_bool
operator==(TimeStamp lhs, TimeStamp rhs) {
  return lhs.getUnixTimeStamp() == rhs.getUnixTimeStamp();
}

inline
qg_bool
operator>(TimeStamp lhs, TimeStamp rhs) {
  return lhs.getUnixTimeStamp() > rhs.getUnixTimeStamp();
}

inline
TimeStamp addTime(TimeStamp timeStamp, qg_double seconds) {
  auto delta = static_cast<qg_int64_t >(seconds);
  return TimeStamp(timeStamp.getUnixTimeStamp() + delta);
}

}//namespace qg

#endif //SRC_TIME_STAMP_H

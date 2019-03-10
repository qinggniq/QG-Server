//
// Created by wc on 3/6/19.
//

#ifndef SRC_TIME_STAMP_H
#define SRC_TIME_STAMP_H

#include "type.h"
#include "copyable.h"

namespace qg{
class Copyable;
class TimeStamp:public Copyable {
 public:
  TimeStamp();
  explicit TimeStamp(const qg_int64_t unixTimeStamp);

  qg_string toString() const;
  qg_string toFormatedString() const;

  qg_int64_t GetUnixTimeStamp() const {return unix_time_stamp_;}

  static TimeStamp Now();


 private:
  qg_int64_t unix_time_stamp_;

};

inline
qg_bool
operator<(TimeStamp lhs, TimeStamp rhs) {
  return lhs.GetUnixTimeStamp() < rhs.GetUnixTimeStamp();
}

inline
qg_bool
operator==(TimeStamp lhs, TimeStamp rhs) {
  return lhs.GetUnixTimeStamp() == rhs.GetUnixTimeStamp();
}

inline
qg_bool
operator>(TimeStamp lhs, TimeStamp rhs) {
  return lhs.GetUnixTimeStamp() > rhs.GetUnixTimeStamp();
}

inline
TimeStamp addTime(TimeStamp timeStamp, qg_double seconds) {
  qg_int64_t delta = static_cast<qg_int64_t >(seconds);
  return TimeStamp(timeStamp.GetUnixTimeStamp() + delta);
}

}//namespace qg

#endif //SRC_TIME_STAMP_H

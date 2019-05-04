//
// Created by wc on 3/6/19.
//

#ifndef SRC_TIMER_H
#define SRC_TIMER_H

#include "type.h"
#include "time_stamp.h"
#include "noncopyable.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <queue>
#include <chrono>


namespace qg{

 class TimerHeap;
 class Timer : public noncopyable {
  public :
   typedef boost::function<void()> timer_handler_cb_t;
   explicit Timer(TimeStamp when, const timer_handler_cb_t& call_back, qg_time_t interval);

   TimeStamp expire() const {return expire_;}
   qg_bool cycle() const {return cycle_;}
   qg_bool timeout() const {return now() > expire();}
   static TimeStamp  now();

   void update(int timeout);
   void run();

   bool operator<(const Timer& rhs) { return expire() < rhs.expire();}

  private:
   friend class TimerHeap;
   TimeStamp expire_;

   qg_bool cycle_;
   qg_time_t interval_;

   timer_handler_cb_t timer_hander_;

   //qg_ssize_t heap_idx_;
 };






}//namespace qg
#endif //SRC_TIMER_H

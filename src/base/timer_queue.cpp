//
// Created by wc on 5/4/19.
//
#include <sys/timerfd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "timer_queue.h"
#include "timer.h"
#include "timer_heap.h"
#include "time_stamp.h"
#include "event_handler.h"
#include "reactor.h"


namespace qg{
namespace timer_tool{
qg_fd_t createTimerfd(){
  qg_fd_t timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
  if (timer_fd == -1) {
	//TODO log ("create Timer failed!")
  }
  return timer_fd;
}

void readTimerfd(qg_fd_t fd) {
  qg_uint64_t exp;
  qg_ssize_t s = ::read(fd, &exp, sizeof(qg_uint64_t));
  if (s != sizeof(qg_uint64_t)) {
	//TODO log ("read tiemrfd failed")
  }
}

void resetTimerfd(qg_fd_t fd, TimeStamp new_time) {
  struct itimerspec new_value, old_value;
  struct timespec ts;
  memset(&new_value, 0, sizeof(struct itimerspec));
  memset(&old_value, 0, sizeof(struct itimerspec));
  auto diff = new_time.getUnixTimeStamp() - TimeStamp::Now().getUnixTimeStamp();
  std::cout << diff << std::endl;
  diff = diff < 100 ? 100 : diff;
  ts.tv_sec = static_cast<qg_time_t >(diff / TimeStamp::kMicroSecondsPerSecond);
  ts.tv_nsec =static_cast<qg_time_t >((diff % TimeStamp::kMicroSecondsPerSecond) * 1000);

  new_value.it_value = ts;
  std::cout << "before :: " << (new_value.it_value.tv_sec) << std::endl;
  if ((::timerfd_settime(fd, 0, &new_value, NULL)) != 0) {
	//TODO log
	 std::cout << new_time.toString() << std::endl;
  }
  //std::cout << "success " <<  new_time.getUnixTimeStamp() << std::endl;
  //TODO fininsh the logic of reset timer;
  struct itimerspec t_value;

  timerfd_gettime((fd), &t_value);
  std::cout <<  "after :: " << (t_value.it_value.tv_sec) << std::endl;

}
}

TimerQueue::TimerQueue(qg::TimerQueue::event_loop_pt el):
loop_(el),
timer_fd_(timer_tool::createTimerfd()),
handler_(new EventHandler(el, timer_fd_)),
timer_heap_(TimerHeap()) {
  handler_->SetReadCallBack(std::bind(&TimerQueue::handleTimer, this));
  loop_->registerHandler(handler_);
}

TimerQueue::~TimerQueue() {
  loop_->removeHandler(handler_);
  ::close(timer_fd_);
  while (!timer_heap_.empty()) {
    timer_heap_.popTimer();
  }
}

void TimerQueue::addTimer(qg::TimerQueue::timer_pt timer) {
  if (_push(timer)) {
    auto new_time = timer->expire();
    timer_tool::resetTimerfd(timer_fd_, new_time);
  }
}

void TimerQueue::handleTimer() {
  TimeStamp now(TimeStamp::Now());
  timer_tool::readTimerfd(timer_fd_);

  while(!timer_heap_.empty()) {
    auto latestTimer = timer_heap_.top();
    if (now > latestTimer->expire()) {
      if (latestTimer->cycle()) {
        latestTimer->restart();
        timer_heap_.popTimer();
        timer_heap_.addTimer(latestTimer);
      }else{
        timer_heap_.popTimer();
      }
      latestTimer->run();
    }else{
      timer_tool::resetTimerfd(timer_fd_, latestTimer->expire());
      return;
    }
  }
}




qg_bool TimerQueue::_push(qg::TimerQueue::timer_pt timer) {
  auto oldTop = timer_heap_.top();
  timer_heap_.addTimer(timer);
  if (oldTop != nullptr) {
    return *timer < *oldTop;
  }
  return true;
}
}

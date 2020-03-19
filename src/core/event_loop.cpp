//
// Created by wc on 1/23/19.
//
#include "event_loop.h"
#include "event_handler.h"

#ifdef __APPLE__
#include "./poll/Pollpoller.h"
#define POLL Pollpoller
#elif __linux__
#include "./poll/Epollpoller.h"
#defing POLL Epollpoller
#endif
#include "../base/timer_queue.h"
#include <cassert>

namespace qg {

EventLoop::EventLoop()
    : poller_(new POLL()), stop_(false),
      defaultPollSize(10) // TODO: default size should be configable
{
  timer_queue_ = std::make_unique<TimerQueue>();
}

EventLoop::~EventLoop() {
  for (handler_map_t::const_iterator it = handler_map_.begin();
       it != handler_map_.end(); ++it) {
    poller_->removeHandler(it->second);
    delete it->second;
  }
}

void EventLoop::registerHandler(qg::EventLoop::event_handler eh) {
  this->handler_map_.emplace(eh->GetHandle(), eh);
  this->poller_->registerHandler(eh);
}

void EventLoop::updateHandler(event_handler eh) {
  this->poller_->updateHandler(eh);
}

void EventLoop::removeHandler(qg::EventLoop::event_handler eh) {
  auto it = this->handler_map_.find(eh->GetHandle());
  assert(it != handler_map_.end());
  handler_map_.erase(it);
  this->poller_->removeHandler(eh);
}

void EventLoop::loop() {
  for (;;) {
    timer_queue_->handleTimer();
    // 这里需要根据time_queue来确定等待时间
    TimeStamp next_time = timer_queue_->getNextTimeStamp();
    if (next_time.getUnixTimeStamp() != 0) {
      TimeStamp now = TimeStamp::Now();
      if (now < next_time) {
        next_time = next_time - now;
      }
    }
    std::vector<EventHandler *> ess = poller_->Wait(defaultPollSize, next_time);
    for (const auto &it : ess) {
      it->HandleEvent();
    }
  }
}

void EventLoop::stop() {}

} // namespace qg
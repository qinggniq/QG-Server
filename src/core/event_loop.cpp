//
// Created by wc on 1/23/19.
//
#include "event_loop.h"
#include "event_handler.h"
//#ifdef __APPLE__
//#include "./poll/KqueuePoller.h"
//#define POLL KqueuePoller
#ifdef __linux__
#include "./poll/Epollpoller.h"
#defing POLL Epollpoller
#else
#include "./poll/Pollpoller.h"
#define POLL Pollpoller
#endif
#include "../base/timer.h"
#include "../base/timer_queue.h"
#include <cassert>
#include <glog/logging.h>
#include <iostream>

namespace qg {

EventLoop::EventLoop()
    : poller_(new POLL()), stop_(false),
      pending_functors_(new Queue<EventLoop::functor_t>()),
      defaultPollSize(10) // TODO: default size should be configable
{
  LOG(INFO) << "event loop construct";
  timer_queue_ = std::make_unique<TimerQueue>();
}

EventLoop::~EventLoop() {
  for (handler_map_t::const_iterator it = handler_map_.begin();
       it != handler_map_.end(); ++it) {
    poller_->removeHandler(it->second);
    delete it->second;
  }
}

void EventLoop::runAfter(qg_time_t time, timer_handler_cb_t cb) {
  timer_queue_->addTimer(std::make_shared<Timer>(
      addTime(TimeStamp::Now(), time), 0, std::move(cb), false));
}

void EventLoop::runAt(TimeStamp time, timer_handler_cb_t cb) {
  timer_queue_->addTimer(
      std::make_shared<Timer>(time, static_cast<qg_time_t>(0), cb, false));
}

void EventLoop::runEvery(qg_time_t interval, timer_handler_cb_t cb) {
  timer_queue_->addTimer(
      std::make_shared<Timer>(TimeStamp::Now(), interval, cb, true));
}

void EventLoop::registerHandler(qg::EventLoop::event_handler_pt eh) {
  assert(eh != nullptr);
  this->handler_map_.emplace(eh->getHandle(), eh);
  this->poller_->registerHandler(eh);
}

void EventLoop::updateHandler(event_handler_pt eh) {
  assert(eh != nullptr);
  this->poller_->updateHandler(eh);
}

void EventLoop::removeHandler(qg::EventLoop::event_handler_pt eh) {
  assert(eh != nullptr);
  auto it = this->handler_map_.find(eh->getHandle());
  assert(it != handler_map_.end());
  handler_map_.erase(it);
  this->poller_->removeHandler(eh);
}

void EventLoop::addFunctor(EventLoop::functor_t functor) {
  pending_functors_->push(functor);
}

void EventLoop::handlePendingFunctors() {
  std::queue<functor_t> functors;
  pending_functors_->pop_all(functors);
  while (!functors.empty()) {
    functors.front()();
    functors.pop();
  }
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
    LOG(INFO) << "loop at " << this;
    std::vector<EventHandler *> ess = poller_->Wait(-1, next_time);
    LOG(INFO) << "loop end with " << ess.size() << " events at " << this;
    for (const auto &it : ess) {
      LOG(INFO) << it->toString();
      it->HandleEvent();
    }
    handlePendingFunctors();
    LOG(INFO) << "loop end";
  }
}
} // namespace qg
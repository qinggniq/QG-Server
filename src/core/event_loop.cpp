//
// Created by wc on 1/23/19.
//
#include "event_loop.h"
#include "event_handler.h"

#include "../base/timer.h"
#include "../base/timer_queue.h"
#include <cassert>
#include <glog/logging.h>
#include <iostream>

namespace qg {

EventLoop::EventLoop()
    : poller_(new POLL()), stop_(false),
      defaultPollSize(10) // TODO: default size should be configable
{
  LOG(INFO) << "event loop construct";
  timer_queue_ = std::make_unique<TimerQueue>();
  pending_functors_ = std::make_unique<Queue<EventLoop::functor_t>>();
  waker_ = std::make_unique<Waker<POLL>>(this);
  LOG(INFO) << "event loop construct end";
  LOG(INFO) << "test end";
}

void EventLoop::wake_up() {
  waker_->wake_up();
}

EventLoop::~EventLoop() {
  LOG(INFO) << "~EventLoop";
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
  LOG(INFO) << "upadte fd " << eh->getHandle() << " at loop " << this;
  assert(eh != nullptr);
  auto it = this->handler_map_.find(eh->getHandle());
  assert(it != handler_map_.end());
  this->poller_->updateHandler(eh);
}

void EventLoop::removeHandler(qg::EventLoop::event_handler_pt eh) {
  LOG(INFO) << "remove fd " << eh->getHandle() << " at loop " << this;
  assert(eh != nullptr);
  auto it = this->handler_map_.find(eh->getHandle());
  assert(it != handler_map_.end());
  handler_map_.erase(it);
  this->poller_->removeHandler(eh);
}

void EventLoop::addFunctor(EventLoop::functor_t functor) {
  LOG(INFO) << "addFunctor";
  LOG(INFO) << "queue " << pending_functors_;
  LOG(INFO) << "timer " << timer_queue_;
  LOG(INFO) << "waker " << waker_;
  pending_functors_->push(functor);
  waker_->wake_up();
}

void EventLoop::handlePendingFunctors() {
  std::queue<functor_t> functors;
  pending_functors_->pop_all(functors);
  LOG(INFO) << "before handle functions " << this;
  while (!functors.empty()) {
    functors.front()();
    functors.pop();
  }
  LOG(INFO) << "handle over " << this;
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
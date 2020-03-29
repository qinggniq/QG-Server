//
// Created by 王聪 on 2020/3/28.
//

#include "KqueuePoller.h"
#include "../event_handler.h"
#include <glog/logging.h>
#include <sys/event.h>
#include <sys/types.h>

namespace qg {
KqueuePoller::KqueuePoller() {
  kfd_ = kqueue();
  if (kfd_ == -1) {
    LOG(ERROR) << "create kqueue failed";
    ::exit(1);
  }
}
KqueuePoller::~KqueuePoller() { ::close(kfd_); }

void KqueuePoller::registerHandler(Poller::handler h) {
  assert(event_handler_map_.find(h->GetHandle()) == event_handler_map_.end());
  struct ::kevent ke;
  if (h->isReading()) {
    EV_SET(&ke, h->GetHandle(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (::kevent(kfd_, &ke, 1, nullptr, 0, nullptr) == -1) {
      LOG(ERROR) << "register read event to kequeue failed";
    }
  }
  if (h->isWriting()) {
    EV_SET(&ke, h->GetHandle(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (::kevent(kfd_, &ke, 1, nullptr, 0, nullptr) == -1) {
      LOG(ERROR) << "register write event to kequeue failed";
    }
  }
  event_handler_map_[h->GetHandle()] = h;
  events_.push_back(ke);
}

// kqueue的更新逻辑
void KqueuePoller::updateHandler(Poller::handler h) {
  assert(event_handler_map_.find(h->GetHandle()) != event_handler_map_.end());
  struct ::kevent ke;
  if (h->isReading()) {
    EV_SET(&ke, h->GetHandle(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (::kevent(kfd_, &ke, 1, nullptr, 0, nullptr) == -1) {
      LOG(ERROR) << "update read event to kequeue failed";
    }
  }
  if (h->isWriting()) {
    EV_SET(&ke, h->GetHandle(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (::kevent(kfd_, &ke, 1, nullptr, 0, nullptr) == -1) {
      LOG(ERROR) << "update write event to kequeue failed";
    }
  }
}

void KqueuePoller::removeHandler(Poller::handler h) {
  assert(event_handler_map_.find(h->GetHandle()) != event_handler_map_.end());
  struct ::kevent ke;
  if (h->isReading()) {
    EV_SET(&ke, h->GetHandle(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    ::kevent(kfd_, &ke, 1, nullptr, 0, nullptr);
  }
  if (h->isWriting()) {
    EV_SET(&ke, h->GetHandle(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    ::kevent(kfd_, &ke, 1, nullptr, 0, nullptr);
  }
  event_handler_map_.erase(h->GetHandle());
  events_.pop_back();
}

std::vector<Poller::handler> KqueuePoller::Wait(int sz, TimeStamp time_stamp) {
  std::vector<Poller::handler> res;
  int retval, numevents = 0;
  if (sz == -1 || sz > events_.size()) {
    sz = events_.size();
  }
  if (time_stamp.getUnixTimeStamp() != 0) {
    struct ::timespec timeout;
    timeout.tv_sec = time_stamp.toSeconds();
    timeout.tv_nsec = 1000; //先设置为0
    LOG(INFO) << "time out " << timeout.tv_sec;
    retval = ::kevent(kfd_, nullptr, 0, events_.data(), sz, &timeout);
    LOG(INFO) << "return events " << retval;
  } else {
    LOG(INFO) << "wait for event";
    retval = ::kevent(kfd_, nullptr, 0, events_.data(), sz, nullptr);
  }

  if (retval > 0) {
    numevents = retval;
    for (int j = 0; j < numevents; j++) {
      int mask = 0;
      struct ::kevent e = events_[j];
      auto it = event_handler_map_.find(e.ident);
      assert(it != event_handler_map_.end());
      if (e.filter == EVFILT_READ)
        mask |= EventMode::kEventRead;
      if (e.filter == EVFILT_WRITE)
        mask |= EventMode::kEventWrite;
      it->second->SetREvents(mask);
      res.push_back(it->second);
    }
  }
  return res;
}

} // namespace qg
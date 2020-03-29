//
// Created by 王聪 on 2020/3/7.
//

#include "Pollpoller.h"
#include "../event_handler.h"
#include <glog/logging.h>
#include <iostream>
#include <sys/poll.h>

using namespace qg;
Pollpoller::Pollpoller() : Poller() {}
Pollpoller::~Pollpoller() {
  for (auto it = event_handler_map_.begin(); it != event_handler_map_.end();
       it++) {
    delete it->second;
  }
}

void qg::Pollpoller::updateHandler(qg::Poller::handler h) {
  int index = h->Index();
  LOG(INFO) << "index " << index << " size " << fds.size();
  assert(index >= 0 && index < this->fds.size());
  LOG(INFO) << ((event_handler_map_)[h->GetHandle()] == h);
  assert((event_handler_map_)[h->GetHandle()] == h); // can't change the fd.

  this->fds[index].events = h->GetIEvents();
  this->fds[index].revents = 0;
}

void qg::Pollpoller::registerHandler(qg::Poller::handler h) {
  assert(h->Index() < 0);
  h->SetIndex(fds.size());
  event_handler_map_[h->GetHandle()] = h;
  struct pollfd pfd;
  pfd.fd = h->GetHandle();
  pfd.events = h->GetIEvents();
  pfd.revents = 0;
  fds.push_back(pfd);
  event_handler_map_.emplace(h->GetHandle(), h);
}

void qg::Pollpoller::removeHandler(qg::Poller::handler h) {
  int index = h->Index();
  assert(index >= 0 && index < this->fds.size());
  assert(event_handler_map_[h->GetHandle()] == h);
  assert(fds[index].revents == 0);
  LOG(INFO) << "set index from " << event_handler_map_[fds.back().fd]->Index()
            << " to " << index;
  // index 是Eventhandler的东西，在删除的时候我们不能移动位置。。
  event_handler_map_[fds.back().fd]->SetIndex(index);
  event_handler_map_.erase(h->GetHandle());
  std::swap(fds[index], fds.back());
  fds.pop_back();
}

std::vector<qg::Poller::handler> qg::Pollpoller::Wait(int sz,
                                                      TimeStamp time_stamp) {
  //可能需要为0的时刻。。
  std::vector<qg::Poller::handler> res;
  //在这里可能会出现问题，
  qg_int wait_time = -1;
  if (time_stamp.getUnixTimeStamp() != 0) {
    wait_time = time_stamp.toMilliseconds();
  }
  if (sz < 0) {
    sz = fds.size();
  } else {
    sz = sz > fds.size() ? fds.size() : sz;
  }
  int n = ::poll(fds.data(), sz, wait_time);
  if (n < 0) {
    switch (errno) {
    case EINVAL:
      LOG(ERROR) << "ndfs greater than " << FD_SETSIZE;
      break;
    default:
      LOG(ERROR) << "other error";
    }
  }

  for (auto &pfd : fds) {
    if (pfd.revents != 0) {
      LOG(WARNING) << "event coming";
      event_handler_map_t_::const_iterator handler =
          event_handler_map_.find(pfd.fd);
      assert(handler != event_handler_map_.end());
      handler->second->SetREvents(pfd.revents);
      LOG(INFO) << "remove the revents";
      //      pfd.revents = 0;
      res.emplace_back(handler->second);
      n--;
    }
    if (n == 0) {
      break;
    }
  }
  return res;
}

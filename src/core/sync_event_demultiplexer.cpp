//
// Created by wc on 1/24/19.
//

#include "sync_event_demultiplexer.h"
#include "../util/log.h"
#include <iostream>

namespace qg {


SyncEventDemultiplexer::SyncEventDemultiplexer() {
  this->sed_fd_ = epoll_create(EPOLL_CLOEXEC);
}

SyncEventDemultiplexer::~SyncEventDemultiplexer() {
  //TODO (qinggniq) close all fds in epoll
  close(this->sed_fd_);
}

void
SyncEventDemultiplexer::Register(handler &handler) {
  //TODO (qinggniq) add the Error Handler
  struct epoll_event eevent;

  eevent.data.fd = handler.GetHandle();
  eevent.events = handler.GetIEvent();
  std::cout <<  (eevent.events == EPOLLIN) << std::endl;
  std::cout << sed_fd_ << " " << handler.GetHandle() << std::endl;
  qg_int ret = epoll_ctl(sed_fd_, EPOLL_CTL_ADD, handler.GetHandle(), &eevent);
  if (ret == -1) {
    //TODO (qinggniq) error handling [errorn = EEXIST]
  }
  std::cout << "reg" << std::endl;
}

void
SyncEventDemultiplexer::Remove(handler &handler) {
  struct epoll_event eevent;

  eevent.data.fd = handler.GetHandle();
  eevent.events = handler.GetIEvent();
  qg_int ret = epoll_ctl(sed_fd_, EPOLL_CTL_DEL, handler.GetHandle(), &eevent);
  if (ret == -1) {
    //TODO (qinggniq) error handling [errorn = ENOENT]
    //log_level = kLogDebug4;
    //Log(kLogError) << "The event handler is not registered!" << std::endl;
  }
}

void
SyncEventDemultiplexer::Update(handler &handler) {
  struct epoll_event eevent;

  eevent.data.fd = handler.GetHandle();
  eevent.events = handler.GetIEvent();
  qg_int ret = epoll_ctl(sed_fd_, EPOLL_CTL_MOD, handler.GetHandle(), &eevent);
  if (ret == -1) {
    //TODO (qinggniq) error handling [errorn = ENOENT]
  }
}

std::vector<EventStatus>
SyncEventDemultiplexer::Wait() {
  struct epoll_event eevent;
  struct epoll_event evlist[kMaxEventsSize];
  std::vector<EventStatus> res;
  //TODO add the real timeout paramenters
  while (kTrue) {
    qg_int ret = epoll_wait(this->sed_fd_, evlist, kMaxEventsSize, 300);
    for (qg_int i=0; i<ret; ++i) {
      EventStatus es({evlist[i].data.fd, evlist[i].events});
      res.push_back(es);
    }
    if (ret > 0) {
      std::cout << "ret" << std::endl;
      return res;
    }else{
      std::cout << "fuck" << std::endl;
    }
  }

  //epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);

}
}//qg
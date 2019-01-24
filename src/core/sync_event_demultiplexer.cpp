//
// Created by wc on 1/24/19.
//

#include "sync_event_demultiplexer.h"

namespace qg {
SyncEventDemultiplexer::SyncEventDemultiplexer() {
  this->sed_fd_ = epoll_create(EPOLL_CLOEXEC);
}

void SyncEventDemultiplexer::Register(handler_pt &handler) {
  //TODO (qinggniq) add the Error Handler
  struct epoll_event eevent;

  eevent.data.fd = handler->GetHandle();
  eevent.events = handler->GetEvents();
  qg_int ret = epoll_ctl(sed_fd_, EPOLL_CTL_ADD, handler->GetHandle(), &eevent);
  if (ret == -1) {
    //TODO (qinggniq) error handling [errorn = EEXIST]
  }

}

void SyncEventDemultiplexer::Remove(handler_pt &handler) {
  struct epoll_event eevent;

  eevent.data.fd = handler->GetHandle();
  eevent.events = handler->GetEvents();
  qg_int ret = epoll_ctl(sed_fd_, EPOLL_CTL_DEL, handler->GetHandle(), &eevent);
  if (ret == -1) {
    //TODO (qinggniq) error handling [errorn = ENOENT]
  }
}

void SyncEventDemultiplexer::Update(handler_pt &handler) {
  struct epoll_event eevent;

  eevent.data.fd = handler->GetHandle();
  eevent.events = handler->GetEvents();
  qg_int ret = epoll_ctl(sed_fd_, EPOLL_CTL_MOD, handler->GetHandle(), &eevent);
  if (ret == -1) {
    //TODO (qinggniq) error handling [errorn = ENOENT]
  }
}

void SyncEventDemultiplexer::Wait() {
  epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);

}
}//qg
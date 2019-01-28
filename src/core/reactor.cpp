//
// Created by wc on 1/23/19.
//

#include "reactor.h"

#include <cassert>


namespace qg {

void
SockHandler::HandleEvent() {
  //TODO read the Linux Programing Interface to figure out the real logic

}


Dispatcher::Dispatcher() = default;


Dispatcher::~Dispatcher() {
  //TODO (qinggniq) implement the logic of freeing
  for (handler_map_t::const_iterator it = handler_map_.begin();
       it != handler_map_.end(); ++it) {
    //free (it->second);
  }
}


void
Dispatcher::RegisterHandler(qg::Dispatcher::event_handler_pt &eh) {
  this->handler_map_.emplace(eh->GetHandle(), eh);
  //TODO (qinggniq) refactor it, the sync_event_demuliplexer shouldn't know much about the EventHander
  this->sed_impl_->Register(eh);
}

void
Dispatcher::ModHandler(qg::Dispatcher::event_handler_pt &eh) {
  this->sed_impl_->Update(eh);
}

void
Dispatcher::RemoveHandler(qg::Dispatcher::event_handler_pt &eh) {
  //TODO (qinggniq) shoud remove the map ????
  handler_map_t::const_iterator it = this->handler_map_.find(eh->GetHandle());
  if (it != this->handler_map_.end()) {
    this->handler_map_.erase(it);
  } else {
    //TODO (qinggniq) log it.
  }

  this->sed_impl_->Remove(eh);
}

void
Dispatcher::Loop() {
  //TODO (qinggniq) we should wait and handle
  for (;;) {
    std::vector<EventStatus> ess = sed_impl_->Wait();
    for (const auto &it:ess) {
      auto handler_iter = this->handler_map_.find(it.fd);
      assert(handler_iter != this->handler_map_.end());
      handler_iter->second->SetREvents(it.revents);
      handler_iter->second->HandleEvent();
    }
  }
}


}//namespace qg
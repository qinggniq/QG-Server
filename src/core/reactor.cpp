//
// Created by wc on 1/23/19.
//

#include "reactor.h"

#include <cassert>


namespace qg {



Dispatcher::Dispatcher():
sed_impl_(new SyncEventDemultiplexer())
{}


Dispatcher::~Dispatcher() {
  //TODO (qinggniq) implement the logic of freeing
  for (handler_map_t::const_iterator it = handler_map_.begin();
       it != handler_map_.end(); ++it) {
    //free (it->second);
  }
}


void
Dispatcher::registerHandler(qg::Dispatcher::event_handler &eh) {
  this->handler_map_.emplace(eh->GetHandle(), eh);
  //TODO (qinggniq) refactor it, the sync_event_demuliplexer shouldn't know much about the EventHander
  this->sed_impl_->Register(eh);
}

void
Dispatcher::modHandler(qg::Dispatcher::event_handler &eh) {
  this->sed_impl_->Update(eh);
}

void
Dispatcher::removeHandler(qg::Dispatcher::event_handler &eh) {
  //TODO (qinggniq) shoud remove the map ????
  //here don't need use auto&
  auto it = this->handler_map_.find(eh->GetHandle());
  if (it != this->handler_map_.end()) {
    this->handler_map_.erase(it);
  } else {
    //TODO (qinggniq) log it.
  }

  this->sed_impl_->Remove(eh);
}

void
Dispatcher::loop() {
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

void
Dispatcher::stop() {

}


}//namespace qg
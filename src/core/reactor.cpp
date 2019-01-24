//
// Created by wc on 1/23/19.
//

#include "reactor.h"
namespace qg {

Dispatcher::Dispatcher() {

}
Dispatcher::~Dispatcher() {
  for (handler_map_t::const_iterator it = handler_map_.begin();
       it != handler_map_.end(); ++it) {
    //free (it->second);
  }
}
void
Dispatcher::Loop() {
  //TODO (qinggniq) we should wait and handle
  for (;;) {
    sed_impl_->Wait();

  }
}

qg_int
Dispatcher::RegisterHandler(qg::events_t ev, qg::Dispatcher::event_handler_pt &eh) {
  this->handler_map_.emplace(ev, eh);
}

qg_int
Dispatcher::RemoveHandler(qg::events_t ev, qg::Dispatcher::event_handler_pt &eh) {
  handler_map_t::const_iterator it = this->handler_map_.find(ev);
  if (it != this->handler_map_.end()) {
    this->handler_map_.erase(it);
  } else {
    //TODO (qinggniq) log it.
  }
}

}//namespace qg
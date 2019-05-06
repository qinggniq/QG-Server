//
// Created by wc on 1/30/19.
//

#include "event_handler.h"

namespace qg{

EventHandler::EventHandler(dispatcher_pt dp, qg_fd_t fd, events_t ie):
    dispatcher_(std::move(dp)),
    handle_(fd),
    ievents_(ie),
    revents_(EventMode::kEventNone){
}

/*
 * C11 new feature: constructor agent
 */


EventHandler::EventHandler(dispatcher_pt dp, qg_fd_t fd):
EventHandler(std::move(dp), fd, EventMode::kEventRead)
{}

void
EventHandler::HandleEvent() {
  if (this->IsReadable()) {
    this->read_call_back_();
  }
  if (this->IsWriteable()) {
    this->write_call_back_();
  }
  if (this->IsError()) {
    this->error_call_back_();
  }
}

} //namespace qg
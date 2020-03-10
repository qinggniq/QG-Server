//
// Created by wc on 1/30/19.
//

#include "event_handler.h"
#include "event_loop.h"

namespace qg{

EventHandler::EventHandler( qg_fd_t fd, events_t ie):
    handle_(fd),
    ievents_(ie),
    revents_(EventMode::kEventNone){
}

/*
 * C11 new feature: constructor agent
 */
EventHandler::EventHandler( qg_fd_t fd):
EventHandler(fd, EventMode::kEventRead|EventMode::kEventWrite)
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
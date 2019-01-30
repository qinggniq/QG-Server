//
// Created by wc on 1/30/19.
//

#include "event_handler.h"

namespace qg{

EventHandler::EventHandler():
handle_(0),
ievents_(EventMode::kEventNone),
revents_(EventMode::kEventNone){
}

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
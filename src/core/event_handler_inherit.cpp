//
// Created by wc on 2/15/19.
//

#include "event_handler_inherit.h"
#include "http_parser.h"

namespace qg{

EventHandler::EventHandler(qg_int fd, events_t ie=EventMode::kEventNone):
    handle_(fd),
    ievents_(ie),
    revents_(EventMode::kEventNone){
}

void
EventHandler::HandleEvent() {
  if (this->IsReadable()) {
    this->HandleRead();
  }
  if (this->IsWriteable()) {
    this->HandleWrite();
  }
  if (this->IsError()) {
    this->HandleError();
  }
}

}//namespace qg
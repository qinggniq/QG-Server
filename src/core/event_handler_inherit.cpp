//
// Created by wc on 2/15/19.
//

#include "event_handler_inherit.h"
namespace qg{

EventHandler::EventHandler():
    handle_(0),
    ievents_(EventMode::kEventNone),
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
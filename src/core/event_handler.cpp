//
// Created by wc on 1/30/19.
//

#include "./event_handler.h"
#include "./event_loop.h"

namespace qg {

EventHandler::EventHandler(event_loop_pt el, qg_fd_t fd, events_t ie)
    : event_loop_(el), handle_(fd), ievents_(ie),
      revents_(EventMode::kEventNone), index_(-1) {}

/*
 * C11 new feature: constructor agent
 */
EventHandler::EventHandler(event_loop_pt el, qg_fd_t fd)
    : EventHandler(el, fd, EventMode::kEventNone) {}

void EventHandler::HandleEvent() {
  if (this->isReadable() && this->isReading()) {
    if (read_call_back_) {
      this->read_call_back_();
    }
  }
  if (this->isWriteable() && this->isWriting()) {
    if (write_call_back_) {
      this->write_call_back_();
    }
  }
  if (this->isError()) {
    if (error_call_back_) {
      this->error_call_back_();
    }
  }
}
qg_string EventHandler::toString() {
  qg_string res;
  res += "fd : " + std::to_string(handle_) + "\n";
  if (isReadable())
    res += "can read\n";
  if (isWriteable())
    res += "can write\n";
  if (isError())
    res += "error happened\n";
  return res;
}

void EventHandler::update() { event_loop_->updateHandler(this); }

} // namespace qg
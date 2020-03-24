//
// Created by wc on 1/30/19.
//

#ifndef SRC_CORE_EVENT_HANDLER_H_
#define SRC_CORE_EVENT_HANDLER_H_

#include "../util/type.h"
#include <functional>

#ifdef __linux__
#include <sys/epoll.h>
enum EventMode {
  kEventNone = 0x00,
  kEventRead = EPOLLIN,
  kEventWrite = EPOLLOUT,
  kEventError = EPOLLERR,
  kEventAll = EPOLLIN | EPOLLOUT | EPOLLERR,
  kEventMask = 0xff
};
#else

#include <sys/poll.h>

enum EventMode {
  kEventNone = 0x00,
  kEventRead = POLL_IN,
  kEventWrite = POLL_OUT,
  kEventError = POLL_ERR,
  kEventAll = kEventRead | kEventWrite | kEventError,
  kEventMask = 0xff
};

#endif
namespace qg {
typedef qg_uint events_t;
typedef qg_int handle_t;

/* You could not implement the read/write handle of file describer,
 * but you must implement the error handle.
 *
 */
struct EventStatus {
  qg_fd_t fd;
  qg_uint revents;
};

class EventLoop;

class EventHandler {
public:
  typedef std::function<void()> Functor;
  typedef EventLoop *event_loop_pt;

  EventHandler(event_loop_pt el, qg_fd_t fd, events_t ie);

  EventHandler(event_loop_pt el, qg_fd_t fd);

  ~EventHandler() = default;

  virtual void HandleEvent();

  handle_t GetHandle() const;

  void SetHandle(handle_t handle);

  void SetReadCallBack(Functor rcb);

  void SetWriteCallBack(Functor wcb);

  void SetErrorCallBack(Functor ecb);

  events_t &GetIEvents();

  events_t &GetREvents();

  void SetIEvents(const events_t &iev);

  void SetREvents(const events_t &rev);

  void enableRead() {
    SetIEvents(EventMode::kEventRead);
    update();
  }

  void enableWrite() {
    SetIEvents(EventMode::kEventWrite);
    update();
  }

  void enableError() {
    SetIEvents(EventMode::kEventError);
    update();
  }

  bool isReading() { return ievents_ & EventMode::kEventRead; };

  bool isWriting() { return ievents_ & EventMode::kEventWrite; };

  void enableAll() {
    SetIEvents(EventMode::kEventAll);
    update();
  }

  void disableRead() {
    ievents_ &= (~EventMode::kEventRead);
    update();
  }

  void disableWrite() {
    ievents_ &= ~EventMode::kEventWrite;
    update();
  }

  void disableError() {
    ievents_ &= ~EventMode::kEventError;
    update();
  }

  void disableAll() {
    this->ievents_ = EventMode::kEventNone;
    update();
  }

  qg_bool isReadable() const;

  qg_bool isWriteable() const;

  qg_bool isError() const;

  int Index() const { return this->index_; }

  void SetIndex(int index) { this->index_ = index; }

  qg_string toString();

private:
  void update();
  event_loop_pt event_loop_;
  handle_t handle_;
  events_t ievents_; // interest events
  events_t revents_; // the result of sync_event_demuliplexer(epoll) returned
  int index_;        // used for poller to find the suitable position
  Functor read_call_back_;
  Functor write_call_back_;
  Functor error_call_back_;
};

inline void EventHandler::SetHandle(const qg::handle_t handle) {
  this->handle_ = handle;
}

inline handle_t EventHandler::GetHandle() const { return this->handle_; }

inline events_t &EventHandler::GetIEvents() { return this->ievents_; }

inline events_t &EventHandler::GetREvents() { return this->revents_; }

inline void EventHandler::SetIEvents(const qg::events_t &iev) {
  this->ievents_ |= (iev & kEventMask);
}

inline void EventHandler::SetREvents(const qg::events_t &rev) {
  this->revents_ |= (rev & kEventMask);
}

inline void EventHandler::SetReadCallBack(Functor rcb) {
  this->read_call_back_ = std::move(rcb);
}

inline void EventHandler::SetWriteCallBack(Functor wcb) {
  this->write_call_back_ = std::move(wcb);
}

inline void EventHandler::SetErrorCallBack(Functor ecb) {
  this->error_call_back_ = std::move(ecb);
}

inline qg_bool EventHandler::isReadable() const {
  return this->revents_ & EventMode::kEventRead;
}

inline qg_bool EventHandler::isWriteable() const {
  return this->revents_ & EventMode::kEventWrite;
}

inline qg_bool EventHandler::isError() const {
  return this->revents_ & EventMode::kEventError;
}

} // namespace qg
#endif // SRC_CORE_EVENT_HANDLER_H_

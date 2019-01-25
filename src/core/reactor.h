//
// Created by wc on 1/23/19.
//

/*
 * I decide use Dispatcher Design Pattern to implement the multiple
 * connections.
 * Here I cite many open source implement of Dispatcher Design Pattern such as
 * https://github.com/song0071000/Reactor_Implemention,
 * https://github.com/justinzhu/reactor,
 * and the redis implemention.
 * */


#ifndef QG_SERVER_REACTOR_H
#define QG_SERVER_REACTOR_H

#include "../util/type.h"
#include "sync_event_demultiplexer.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace qg {
const qg_int kMaxEventsSize = 10;

class SyncEventDemultiplexer;
typedef qg_uint events_t;
typedef qg_int handle_t;

enum EventMode {
  kEventRead = 0x01,
  kEventWrite = 0x02,
  kEventError = 0x03,
  kEventMask = 0xff
};

/* You could not implement the read/write handle of file describer,
 * but you must implement the error handle.
*
 */


class EventHandler {
 public:
  typedef std::functor<void()> Functor;
  virtual void HandleEvent() = 0;

  handle_t GetHandle() const;
  void SetHandle(const handle_t handle);
  void SetReadCallBack(Functor &rcb);
  void SetWriteCallBack(Functor &wcb);
  void SetErrorCallBack(Functor &ecb);

  events_t GetEvent() const;
  void SetEvent(const events_t ev);

 protected:
  EventHandler() = default;
  virtual ~EventHandler() = 0;

  handle_t handle_;
  events_t events_;
  Functor read_call_back_;
  Functor write_call_back_;
  Functor error_call_back_;
};

class SockHandler : public EventHandler {
 public:
  void HandleEvent();
  SockHandler() = default;
  ~SockHandler() = default;
 private:
  const events_t kNoneEvent = 0x0;
  const events_t kReadEvent = 0x1;
  const events_t kWriteEvent = 0x2;
};

class Dispatcher {
 public:
  typedef std::shared_ptr<EventHandler> event_handler_pt;
  typedef std::shared_ptr<SyncEventDemultiplexer> sync_event_demulp_pt;
  typedef std::unordered_map<events_t, std::shared_ptr<EventHandler>> handler_map_t;

  Dispatcher();
  ~Dispatcher();
  qg_int RegisterHandler(events_t ev, event_handler_pt &eh);
  qg_int RemoveHandler(events_t ev, event_handler_pt &eh);
  //
  void Loop();

 private:
  Dispatcher(const Dispatcher &);
  Dispatcher &operator=(const Dispatcher &);

 private:
  sync_event_demulp_pt sed_impl_;
  handler_map_t handler_map_;
};

inline
void
EventHandler::SetHandle(const qg::handle_t handle) {
  this->handle_ = handle;
}

inline
void
EventHandler::GetHandle() const {
  return this->handle_;
}

inline
void
EventHandler::GetEvent() const {
 return this->events_;
}

inline
void
EventHandler::SetEvent(const qg::events_t ev) {
  this->events_ = ev;
}

inline
void
EventHandler::SetReadCallBack(std::functor<void()> &rcb) {
  this->read_call_back_ = rcb;
}

inline
void
EventHandler::SetWriteCallBack(std::functor<void()> &wcb) {
  this->write_call_back_ = wcb;
}

inline
void
EventHandler::SetErrorCallBack(std::functor<void()> &ecb) {
  this->error_call_back_ = ecb;
}


} //namespace qg
#endif //QG_SERVER_REACTOR_H

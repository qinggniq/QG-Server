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

namespace qg {
typedef qg_uint events_t;

enum EventMode {
  kEventRead = 0x01,
  kEventWrite = 0x02,
  kEventError = 0x03,
  kEventMask = 0xff
};

typedef qg_int handle_t;

/* You could not implement the read/write handle of file describer,
 * but you must implement the error handle.
*
 */


class EventHandler {
 public:
  virtual handle_t GetHandle() const = 0;
  virtual void HandleEvent() = 0;
  virtual event_t GetEvents() const = 0;

 protected:
  EventHandler() = default;
  virtual ~EventHandler() = 0;
  handle_t handle_;
  events_t events_;
};

class SockHandler : public EventHandler {
 public:
  handle_t GetHandle() const override {
    return handle_;
  };
  events_t GetEvents() const override {
    return events_ ;
  }
  void HandleEvent () ;
  SockHandler() = default;
  ~SockHandler() = default;

};

class Dispatcher {
 public:
  typedef std::shared_ptr<EventHandler> event_handler_pt;
  typedef std::shared_ptr<SyncEventDemultiplexer> sync_event_demulp_pt;
  typedef std::unordered_map<event_t, std::shared_ptr<EventHandler>> handler_map_t;

  Dispatcher();
  ~Dispatcher();
  qg_int RegisterHandler(event_t ev, event_handler_pt &eh);
  qg_int RemoveHandler(event_t ev, event_handler_pt &eh);
  //
  void Loop();

 private:
  Dispatcher(const Dispatcher &);
  Dispatcher &operator=(const Dispatcher &);

 private:
  sync_event_demulp_pt sed_impl_;
  handler_map_t handler_map_;
};

} //namespace qg
#endif //QG_SERVER_REACTOR_H

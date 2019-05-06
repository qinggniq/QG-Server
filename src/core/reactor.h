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
const qg_int kMaxEventsSize = 10;

class SyncEventDemultiplexer;


class Dispatcher {
 public:
  typedef EventHandler event_handler;
  typedef std::shared_ptr<SyncEventDemultiplexer> sync_event_demulp_pt;
  typedef std::unordered_map<handle_t, EventHandler*> handler_map_t;


  Dispatcher();
  ~Dispatcher();
  void registerHandler(event_handler &eh);
  void removeHandler(event_handler &eh);
  void modHandler(event_handler &eh);
  //
  void loop();
  void stop();

 private:
  Dispatcher(const Dispatcher &);
  Dispatcher &operator=(const Dispatcher &);

 private:
  sync_event_demulp_pt sed_impl_;
  handler_map_t handler_map_;

};



} //namespace qg
#endif //QG_SERVER_REACTOR_H

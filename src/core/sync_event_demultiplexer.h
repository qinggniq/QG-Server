//
// Created by wc on 1/24/19.
//

#ifndef QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H
#define QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H

#include "type.h"
#include "reactor.h"
#include <sys/epoll.h>
#include <memory>


namespace qg {

/*
 * Here I decide use epoll to implement the demuliplexer.
 *
 * */
class EventHandler;
struct EventStatus;


using handle_t = qg_int;

class SyncEventDemultiplexer {
 public:


  typedef qg_event_t ev_t;
  typedef std::shared_ptr<EventHandler>  handler_pt;


  SyncEventDemultiplexer();
  ~SyncEventDemultiplexer();
  void Register(handler_pt &handler); //Add
  void Update(handler_pt &handler);
  void Remove(handler_pt &handler);
  std::vector<EventStatus> Wait();//TODO
 private:

  handle_t sed_fd_;

};

}//namespace qg
#endif //QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H

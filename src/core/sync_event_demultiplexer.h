//
// Created by wc on 1/24/19.
//

#ifndef QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H
#define QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H


#include <sys/epoll.h>
#include <memory>
#include <vector>
#include "type.h"
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
  typedef std::shared_ptr<EventHandler>  handler;


  SyncEventDemultiplexer();
  ~SyncEventDemultiplexer();
  void Register(handler &handler); //Add
  void Update(handler &handler);
  void Remove(handler &handler);
  std::vector<EventStatus> Wait();//TODO
 private:
  const qg_int kMaxEventsSize = 5;
  handle_t sed_fd_;

};

}//namespace qg
#endif //QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H

//
// Created by wc on 1/24/19.
//

#ifndef QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H
#define QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H
#ifdef _WIN64
//define something for Windows (64-bit)
#elif _WIN32
//define something for Windows (32-bit)
#elif __APPLE__
#include "TargetConditionals.h"
#include <sys/poll.h>
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
// define something for simulator
#elif TARGET_OS_IPHONE
// define something for iphone
#else
#define TARGET_OS_OSX 1
// define something for OSX
#endif
#elif __linux
// linux
#include <sys/epoll.h>
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif

#include <memory>
#include <vector>
#include "type.h"
namespace qg {

/*
 * Here I decide use epoll to implement the demuliplexer when system is in linux
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
  std::vector<EventHandler*> Wait();//TODO
 private:
  const qg_int kMaxEventsSize = 5;
  handle_t sed_fd_;
};

}//namespace qg
#endif //QG_SERVER_SYNC_EVENT_DEMULTIPLEXER_H

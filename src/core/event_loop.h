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

#ifndef QG_SERVER_EVENT_LOOP_H
#define QG_SERVER_EVENT_LOOP_H

#include "../util/type.h"
#include <memory>
#include <unordered_map>

namespace qg {
const qg_int kMaxEventsSize = 10;

class Poller;
class EventHandler;
class TimerQueue;
class EventLoop {
public:
  typedef EventHandler *event_handler;
  typedef Poller *poller_pt;
  typedef std::unordered_map<qg_fd_t, event_handler> handler_map_t;

  EventLoop();

  ~EventLoop();

  void registerHandler(event_handler eh);

  void removeHandler(event_handler eh);

  void updateHandler(event_handler eh);

  void loop();

  void stop();

  EventLoop(const EventLoop &) = delete;

  EventLoop &operator=(const EventLoop &) = delete;

private:
  bool stop_;
  poller_pt poller_;
  const int defaultPollSize;
  std::unique_ptr<TimerQueue> timer_queue_;
  handler_map_t handler_map_;
};
} // namespace qg
#endif // QG_SERVER_EVENT_LOOP_H

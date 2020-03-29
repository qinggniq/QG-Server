//
// Created by wc on 1/23/19.
//

#ifndef QG_SERVER_EVENT_LOOP_H
#define QG_SERVER_EVENT_LOOP_H
#include "../util/time_stamp.h"
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
  typedef EventHandler *event_handler_pt;
  typedef Poller *poller_pt;
  typedef std::unordered_map<qg_fd_t, event_handler_pt> handler_map_t;

  EventLoop();

  ~EventLoop();

  void registerHandler(event_handler_pt eh);

  void removeHandler(event_handler_pt eh);

  void updateHandler(event_handler_pt eh);

  void runAfter(qg_time_t time, timer_handler_cb_t cb);

  void runEvery(qg_time_t interval, timer_handler_cb_t cb);

  void runAt(TimeStamp time, timer_handler_cb_t cb);

  void loop();

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

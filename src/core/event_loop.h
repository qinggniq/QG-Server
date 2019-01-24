//
// Created by wc on 1/23/19.
//

#ifndef QG_SERVER_EVENT_LOOP_H
#define QG_SERVER_EVENT_LOOP_H

#include "../util/type.h"
#include <memory>
#include <functional>

namespace qg {
class Channel;
class Poller;
class TimerQueue;

class EventLoop {
 public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();

  void Loop();
  void Stop();

};
}
#endif //QG_SERVER_EVENT_LOOP_H

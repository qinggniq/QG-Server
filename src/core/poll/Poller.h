//
// Created by 王聪 on 2020/3/7.
//

#ifndef QG_CORE_SERVER_POLLER_H_
#define QG_CORE_SERVER_POLLER_H_
#include "../../util/time_stamp.h"
#include <vector>
#include <map>

namespace qg {
class EventHandler;
class Poller {
public:
  typedef EventHandler *handler;
  Poller() = default;
  virtual ~Poller() {}
  virtual void registerHandler(handler) = 0;
  virtual void removeHandler(handler) = 0;
  virtual void updateHandler(handler) = 0;
  virtual std::vector<handler> Wait(int sz, TimeStamp time_stamp) = 0;

protected:
  typedef std::map<int, EventHandler*> event_handler_map_t_;
  event_handler_map_t_ event_handler_map_;
};
}  // namespace qg
#endif  // QG_SERVER_POLLER_H_

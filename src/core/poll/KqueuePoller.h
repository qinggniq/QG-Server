//
// Created by 王聪 on 2020/3/28.
//

#ifndef QGSERVER_SRC_CORE_POLL_KQUEUEPOLLER_H_
#define QGSERVER_SRC_CORE_POLL_KQUEUEPOLLER_H_

#include "Poller.h"
#include <sys/event.h>
namespace qg {

class KqueuePoller : public Poller {
public:
  KqueuePoller();
  ~KqueuePoller() override;
  void registerHandler(handler) override;
  void updateHandler(handler) override;
  void removeHandler(handler) override;
  std::vector<handler> Wait(int sz, TimeStamp time_stamp) override;

private:
  qg_fd_t kfd_;
  std::vector<struct ::kevent> events_;
};

} // namespace qg

#endif // QGSERVER_SRC_CORE_POLL_KQUEUEPOLLER_H_

//
// Created by 王聪 on 2020/3/7.
//

#include "Pollpoller.h"
#include "../event_handler.h"
#include <sys/poll.h>

using namespace qg;
void qg::Pollpoller::updateHandler(qg::Poller::handler h) {
  int index = h->Index();
  assert(index >= 0 && index < this->fds.size());
  assert((*mp)[h->GetHandle()] == h); // can't change the fd.
  this->fds[index].events = h->GetIEvents();
  this->fds[index].revents = 0;
}

void qg::Pollpoller::registerHandler(qg::Poller::handler h) {
  assert(h->Index() < 0);
  h->SetIndex(fds.size());
  struct pollfd pfd;
  pfd.fd = h->GetHandle();
  pfd.events = h->GetIEvents();
  pfd.revents = 0;
  fds.push_back(pfd);
  mp->emplace(h->GetHandle(), h);
}

void qg::Pollpoller::removeHandler(qg::Poller::handler h) {
  int index = h->Index();
  assert(index >= 0 &&
         index < this->fds.size()); // can't remove does not exists;
  assert((*mp)[h->GetHandle()] == h);
  assert(fds[index].revents ==
         0); // can't remove when there are events not deal with
  mp->erase(h->GetHandle());

  std::swap(fds[index], fds.back());
  fds.pop_back();
}

std::vector<qg::Poller::handler> qg::Pollpoller::Wait(int sz,
                                                      TimeStamp time_stamp) {
  assert(sz > 0); //可能需要为0的时刻。。
  std::vector<qg::Poller::handler> res;
  //在这里可能会出现问题，
  qg_int wait_time = -1;
  if (time_stamp.getUnixTimeStamp() != 0) {
    wait_time = time_stamp.toMilliseconds();
  }
  ::poll(fds.data(), fds.size(), wait_time);
  for (auto &pfd : fds) {
    if (pfd.revents != 0) {
      handler_map_t::const_iterator handler = (*mp).find(pfd.fd);
      assert(handler != mp->end());
      handler->second->SetREvents(pfd.revents);
      res.emplace_back(handler->second);
      sz--;
    }
    if (sz == 0) {
      break;
    }
  }
  return res;
}

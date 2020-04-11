//
// Created by wc on 1/23/19.
//

#ifndef QG_SERVER_EVENT_LOOP_H
#define QG_SERVER_EVENT_LOOP_H

//#ifdef __APPLE__
//#include "./poll/KqueuePoller.h"
//#define POLL KqueuePoller
#ifdef __linux__
#include "./poll/Epollpoller.h"
#defing POLL Epollpoller
#else
#include "./poll/Pollpoller.h"
#define POLL Pollpoller
#endif
#include "../util/block_queue.h"
#include "../util/nettool.h"
#include "../util/rwtool.h"
#include "../util/time_stamp.h"
#include "../util/type.h"
#include "event_handler.h"
#include <memory>
#include <unistd.h>
#include <unordered_map>

namespace qg {
const qg_int kMaxEventsSize = 10;
class EventLoop;

class Poller;
class EventHandler;
class TimerQueue;
template <typename T> class Waker;
class EventLoop {
public:
  typedef std::function<void()> functor_t;
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

  void addFunctor(functor_t);
  void handlePendingFunctors();
  void loop();

  void wake_up();

  EventLoop(const EventLoop &) = delete;

  EventLoop &operator=(const EventLoop &) = delete;

private:
  bool stop_;
  poller_pt poller_;
  const int defaultPollSize;
  std::unique_ptr<Queue<functor_t>> pending_functors_;
  std::unique_ptr<TimerQueue> timer_queue_;
  std::unique_ptr<Waker<POLL>> waker_;
  handler_map_t handler_map_;
};

template <typename T> class Waker {
public:
  Waker(EventLoop *loop) : loop_(loop) {
    ::pipe(pipe_);
    makeFdNoneBlock(getReadFd());
    makeFdNoneBlock(getWriteFd());
    event_handler_ = new EventHandler(loop_, getReadFd());
    event_handler_->setReadCallBack(std::bind(&Waker<T>::handleRead, this));
    loop_->registerHandler(event_handler_);
    event_handler_->enableRead();
  }
  ~Waker() {
    event_handler_->disableAll();
    loop_->removeHandler(event_handler_);
    ::close(pipe_[0]);
    ::close(pipe_[1]);
    delete event_handler_;
  }

  void wake_up() {
    LOG(INFO) << "wake up a thread fd : " << getReadFd();
    int one = 1;
    ::write(getWriteFd(), &one, 1);
  }

private:
  qg_fd_t getWriteFd() const { return pipe_[1]; }
  qg_fd_t getReadFd() const { return pipe_[0]; }
  void handleRead() {
    LOG(INFO) << "consume a msg fd : " << getReadFd();
    char buf[1];
    int nread = ::read(getReadFd(), buf, 1);
    if (nread < 0) {
      LOG(ERROR) << "error in pipe read";
    }
  }
  EventLoop *loop_;
  EventHandler *event_handler_;
  qg_fd_t pipe_[2];
};

class EpollPoller;
template <> class Waker<EpollPoller> {
public:
  Waker(EventLoop *loop) {}
  ~Waker() = default;
  void wake_up() {}

private:
  qg_fd_t getWriteFd() const {
    assert(0 && "impossible path");
    return -1;
  }
  qg_fd_t getReadFd() const {
    assert(0 && "impossible path");
    return -1;
  }
  void handleRead() {}
};
} // namespace qg
#endif // QG_SERVER_EVENT_LOOP_H

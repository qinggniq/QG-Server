//
// Created by wc on 1/23/19.
//
#include "event_loop.h"
#include "event_handler.h"

#ifdef __APPLE__

#include "./poll/Pollpoller.h"

#define POLL Pollpoller
#elif __linux__
#include "./poll/Epollpoller.h"
#defing POLL Epollpoller
#endif

#include <cassert>

namespace qg {


    EventLoop::EventLoop() :
            poller_(new POLL()), stop_(false), defaultPollSize(10) //TODO: default size should be configable
    {
    }


    EventLoop::~EventLoop() {
        //TODO (qinggniq) implement the logic of freeing
        for (handler_map_t::const_iterator it = handler_map_.begin();
             it != handler_map_.end(); ++it) {
            poller_->removeHandler(it->second);
            delete it->second;
        }
    }


    void
    EventLoop::registerHandler(qg::EventLoop::event_handler eh) {
        this->handler_map_.emplace(eh->GetHandle(), eh);
        this->poller_->registerHandler(eh);
    }

    void
    EventLoop::modHandler(qg::EventLoop::event_handler eh) {
        this->poller_->updateHandler(eh);
    }

    void
    EventLoop::removeHandler(qg::EventLoop::event_handler eh) {
        //TODO (qinggniq) shoud remove the map ????
        auto it = this->handler_map_.find(eh->GetHandle());
        assert(it != handler_map_.end());
        handler_map_.erase(it);
        this->poller_->removeHandler(eh);
    }

    void
    EventLoop::loop() {
        //TODO (qinggniq) we should wait and handle
        assert(!stop_);
        for (; stop_;) {
            std::vector<EventHandler *> ess = poller_->Wait(defaultPollSize);
            for (const auto &it:ess) {
                it->HandleEvent(); //TODO: 在增加了多线程之后需要把这个处理移动到计算线程里面去。
            }
        }
    }

    void
    EventLoop::stop() {

    }


}//namespace qg
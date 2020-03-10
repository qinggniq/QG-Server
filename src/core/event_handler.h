//
// Created by wc on 1/30/19.
//

#ifndef SRC_EVENT_HANDLER_H
#define SRC_EVENT_HANDLER_H

#include <functional>

#include "../util/type.h"

#ifdef __linux__
#include <sys/epoll.h>
enum EventMode {
  kEventNone=0x00,
  kEventRead = EPOLLIN ,
  kEventWrite = EPOLLOUT,
  kEventError = EPOLLERR,
  kEventAll = EPOLLIN | EPOLLOUT | EPOLLERR,
  kEventMask = 0xff
};
#else

#include <sys/poll.h>

enum EventMode {
    kEventNone = 0x00,
    kEventRead = POLL_IN,
    kEventWrite = POLL_OUT,
    kEventError = POLL_ERR,
    kEventAll = kEventRead | kEventWrite | kEventError,
    kEventMask = 0xff
};

#endif
namespace qg {
    typedef qg_uint events_t;
    typedef qg_int handle_t;


/* You could not implement the read/write handle of file describer,
 * but you must implement the error handle.
*
 */
    struct EventStatus {
        qg_fd_t fd;
        qg_uint revents;
    };

    class Dispatcher;

    class EventLoop;

    class EventHandler {
    public:
        typedef std::function<void()> Functor;
        typedef EventLoop *event_loop_pt;

        EventHandler(qg_fd_t fd, events_t ie);

        EventHandler(qg_fd_t fd);

        ~EventHandler() = default;

        virtual void HandleEvent();

        handle_t GetHandle() const;

        void SetHandle(handle_t handle);

        void SetReadCallBack(Functor rcb);

        void SetWriteCallBack(Functor wcb);

        void SetErrorCallBack(Functor ecb);

        events_t &GetIEvents();

        events_t &GetREvents();

        void SetIEvents(const events_t &iev);

        void SetREvents(const events_t &rev);

        void enableRead() { SetIEvents(EventMode::kEventRead); }

        void enableWrite() { SetIEvents(EventMode::kEventWrite); }

        void enableError() { SetIEvents(EventMode::kEventError); }

        void enableAll() { SetIEvents(EventMode::kEventAll); }

        void disableRead() { ievents_ &= (~EventMode::kEventRead); }

        void disableWrite() { ievents_ &= ~EventMode::kEventWrite; }

        void disableError() { ievents_ &= ~EventMode::kEventError; }

        void disableAll() { this->ievents_ = EventMode::kEventNone; }

        qg_bool IsReadable() const;

        qg_bool IsWriteable() const;

        qg_bool IsError() const;

        int Index() const { return this->index_; }

        void SetIndex(int index) { this->index_ = index; }

    protected:
        handle_t handle_;
        events_t ievents_; //interest events
        events_t revents_; //the result of sync_event_demuliplexer(epoll) returned
        int index_; //used for poller to find the suitable position
        Functor read_call_back_;
        Functor write_call_back_;
        Functor error_call_back_;
    };


    inline
    void
    EventHandler::SetHandle(const qg::handle_t handle) {
        this->handle_ = handle;
    }

    inline
    handle_t
    EventHandler::GetHandle() const {
        return this->handle_;
    }

    inline
    events_t &
    EventHandler::GetIEvents() {
        return this->ievents_;
    }

    inline
    events_t &
    EventHandler::GetREvents() {
        return this->revents_;
    }

    inline
    void
    EventHandler::SetIEvents(const qg::events_t &iev) {
        this->ievents_ |= (iev & kEventMask);
    }

    inline
    void
    EventHandler::SetREvents(const qg::events_t &rev) {
        this->revents_ |= (rev & kEventMask);
    }

    inline
    void
    EventHandler::SetReadCallBack(Functor rcb) {
        this->read_call_back_ = rcb;
    }

    inline
    void
    EventHandler::SetWriteCallBack(Functor wcb) {
        this->write_call_back_ = wcb;
    }

    inline
    void
    EventHandler::SetErrorCallBack(Functor ecb) {
        this->error_call_back_ = ecb;
    }

    inline
    qg_bool
    EventHandler::IsReadable() const {
        return static_cast<qg_bool>(this->revents_ & EventMode::kEventRead);
    }

    inline
    qg_bool
    EventHandler::IsWriteable() const {
        return static_cast<qg_bool>(this->revents_ & EventMode::kEventWrite);
    }

    inline
    qg_bool
    EventHandler::IsError() const {
        return static_cast<qg_bool>(this->revents_ & EventMode::kEventError);
    }


}//namespace qg
#endif //SRC_EVENT_HANDLER_H

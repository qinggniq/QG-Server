//
// Created by wc on 5/6/19.
//

#ifndef SRC_TCP_CONNECTION_H
#define SRC_TCP_CONNECTION_H

#include <functional>
#include "../util/type.h"
#include "../util/noncopyable.h"

namespace qg {
    class EventLoop;

    class EventHandler;

    class Socket;

class TcpConnection : public noncopyable, std::enable_shared_from_this<TcpConnection>{
    public:
        typedef std::string *buf_pt;
        typedef Socket *socket_pt;
        typedef EventLoop *event_loop_pt;
        typedef EventHandler *event_handler_pt;
        typedef std::function<void(TcpConnection *, buf_pt, buf_pt)> HandleReadCallBack;
        typedef std::function<void()> HandleWriteCallBack;

        explicit TcpConnection(event_loop_pt el, qg_fd_t fd);
        explicit TcpConnection(event_loop_pt el, socket_pt sock);
        void handleConnection();

        void handleRead();

        void handleWrite();

        void handleError();

        void write(buf_pt str);

        event_handler_pt eventHandler() { return event_handler_; }

        static int defaultReadBuffSize;
    private:
        socket_pt socket_;
        event_handler_pt event_handler_;
        event_loop_pt event_loop_;
        HandleReadCallBack read_cb_;
        buf_pt read_buf_;
        buf_pt write_buf_;
        //可能需要定时器 -》 用于清除时间过长未响应的客户端连接
        //需要一些缓存相关的结构，用于存储客户端发过来的数据。
    };
}//namespace qg

#endif //SRC_TCP_CONNECTION_H

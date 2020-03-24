//
// Created by wc on 5/6/19.
//

#ifndef SRC_TCP_CONNECTION_H
#define SRC_TCP_CONNECTION_H

#include "../util/noncopyable.h"
#include "../util/type.h"
#include <functional>

namespace qg {
class EventLoop;

class EventHandler;

class Socket;
//这里设置TcpConnection为enable_shared_from_this是为了控制tcpconnection的生命周期，避免Connection在
// handleEvent的时候被析构。
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
  typedef std::string *buf_pt;
  typedef Socket *socket_pt;
  typedef EventLoop *event_loop_pt;
  typedef EventHandler *event_handler_pt;

  TcpConnection(event_loop_pt el, std::unique_ptr<Socket> socket);

  ~TcpConnection();

  void handleConnection();

  void handleRead();

  void handleWrite();

  void setMessageCallBack(MessageCallBack cb) {
    this->message_call_back_ = std::move(cb);
  }
  void setWriteCompleteCallBack(WriteCompleteCallBack cb) {
    this->write_complete_call_back_ = std::move(cb);
  }
  void setConnectionCallBack(ConnectionCallBack cb) {
    this->connection_call_back_ = std::move(cb);
  }
  void write(buf_pt str);

  event_handler_pt eventHandler() { return event_handler_; }

  static int defaultReadBuffSize;

private:
  event_loop_pt event_loop_;
  std::unique_ptr<Socket> socket_;
  event_handler_pt event_handler_;
  buf_pt read_buf_;
  buf_pt write_buf_;
  MessageCallBack message_call_back_;
  WriteCompleteCallBack write_complete_call_back_;
  ConnectionCallBack connection_call_back_;

  //可能需要定时器 -》 用于清除时间过长未响应的客户端连接
  //需要一些缓存相关的结构，用于存储客户端发过来的数据。
};
} // namespace qg

#endif // SRC_TCP_CONNECTION_H

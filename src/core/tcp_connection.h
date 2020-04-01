//
// Created by wc on 5/6/19.
//

#ifndef SRC_CORE_TCP_CONNECTION_H_
#define SRC_CORE_TCP_CONNECTION_H_
#include "../util/noncopyable.h"
#include "../util/type.h"
#include "./socket.h"
#include <any>
#include <functional>
#include <glog/logging.h>
#include <string>
#include <utility>

namespace qg {
class EventLoop;

class EventHandler;

class Accepter;

// 这里设置TcpConnection为enable_shared_from_this是为了控制tcpconnection的生命周期，避免Connection在
// handleEvent的时候被析构。
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
  typedef std::string *buf_pt;
  typedef Socket *socket_pt;
  typedef EventLoop *event_loop_pt;
  typedef EventHandler *event_handler_pt;

  explicit TcpConnection(event_loop_pt el, std::unique_ptr<Socket> socket,
                         Accepter *accpeter);

  ~TcpConnection();

  void handleConnection();

  void handleRead();

  void handleWrite();

  void handleClose();

  qg_fd_t fd() const { return socket_->sfd(); }

  void setMessageCallBack(MessageCallBack cb) {
    this->message_call_back_ = std::move(cb);
  }
  void setWriteCompleteCallBack(WriteCompleteCallBack cb) {
    this->write_complete_call_back_ = std::move(cb);
  }
  void setConnectionComeCallBack(ConnectionComeCallBack cb) {
    this->connection_come_call_back_ = std::move(cb);
  }
  void setConnectionCloseCallBack(ConnectionCloseCallBack cb) {
    this->connection_close_call_back_ = std::move(cb);
  }
  std::any context() const { return context_; }
  void setContext(std::any context) { context_ = std::move(context); }
  void write(buf_pt str);
  void shutdown() {
    LOG(ERROR) << "have not implement";
    assert(false);
  }

  event_handler_pt eventHandler() { return event_handler_; }

  static int defaultReadBuffSize;

private:
  event_loop_pt event_loop_;
  std::unique_ptr<Socket> socket_;
  event_handler_pt event_handler_;
  Accepter *accpeter_;
  bool disconnected_;
  buf_pt read_buf_;
  buf_pt write_buf_;
  std::any context_;
  MessageCallBack message_call_back_;
  WriteCompleteCallBack write_complete_call_back_;
  ConnectionComeCallBack connection_come_call_back_;
  ConnectionCloseCallBack connection_close_call_back_;
};
} // namespace qg

#endif // SRC_CORE_TCP_CONNECTION_H_

//
// Created by wc on 5/7/19.
//

#ifndef SRC_ACCPETOR_H
#define SRC_ACCPETOR_H
#include "../util/noncopyable.h"
#include "../util/type.h"
#include <map>
#include <memory>
#include <utility>

namespace qg {

class EventLoop;

class Socket;

class EventHandler;

class Config;

class Accepter;

class TcpConnection;

class LoopThreadPool;

// server
// 是唯一暴露给用户的东西，所以注册"写回调、读回调、连接回调"需要通过Server来注册
class Server : public qg::noncopyable {
public:
  typedef EventLoop *event_loop_pt;
  typedef std::unique_ptr<EventHandler> event_handler_pt;
  typedef Socket *socket_pt;
  typedef Config *config_pt;
  typedef Accepter *accepter_pt;
  typedef std::map<qg_fd_t, std::shared_ptr<TcpConnection>> mp_t;

  explicit Server(config_pt config);

  ~Server();

  void run();

  void handleNewCon(std::unique_ptr<Socket>);
  void handleConnectionClose(std::shared_ptr<TcpConnection>);
  void closeAtBaseLoop(std::shared_ptr<TcpConnection>);
  void setMessageCallback(MessageCallBack cb) {
    message_call_back_ = std::move(cb);
  }

  void setWriteCompleteCallBack(WriteCompleteCallBack cb) {
    write_complete_call_back_ = std::move(cb);
  }

  void setConnectionComeCallBack(ConnectionComeCallBack cb) {
    connection_come_call_back_ = std::move(cb);
  }
  void setConnectionCloseCallBack(ConnectionCloseCallBack cb) {
    connection_close_call_back_ = std::move(cb);
  }

private:
  static int thread_num;
  event_loop_pt event_loop_;
  std::unique_ptr<LoopThreadPool> loop_pool_;
  accepter_pt accepter_;
  mp_t connections_;
  MessageCallBack message_call_back_;
  WriteCompleteCallBack write_complete_call_back_;
  ConnectionComeCallBack connection_come_call_back_;
  ConnectionCloseCallBack connection_close_call_back_;
};

} // namespace qg

#endif // SRC_ACCPETOR_H

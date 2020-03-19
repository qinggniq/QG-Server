//
// Created by wc on 5/7/19.
//

#ifndef SRC_ACCPETOR_H
#define SRC_ACCPETOR_H
#include <memory>
#include <map>
#include "../util/noncopyable.h"
#include "../util/type.h"


namespace qg {

class EventLoop;

class Socket;

class EventHandler;

class Config;

class Accepter;

class TcpConnection;

// server 是唯一暴露给用户的东西，所以注册"写回调、读回调、连接回调"需要通过Server来注册
class Server : public qg::noncopyable {
public:
  typedef EventLoop *event_loop_pt;
  typedef EventHandler *event_handler_pt;
  typedef Socket *socket_pt;
  typedef Config *config_pt;
  typedef Accepter* accepter_pt;
  typedef std::map<qg_fd_t, TcpConnection*> mp_t;
  //需要将缓冲区的定义移到外面
  typedef std::string* buf_pt;
  typedef std::function<void(std::shared_ptr<TcpConnection>, buf_pt)> MessageCallBack;
  typedef std::function<void(std::shared_ptr<TcpConnection>, buf_pt)>
      WriteCompleteCallBack; // TODO(qinggni), ERROR;
  typedef std::function<void(std::shared_ptr<TcpConnection>)> ConnectionCallBack;
  typedef std::function<void()> HandleWriteCallBack;

  explicit Server(event_loop_pt event_loop, config_pt config);

  void run();

  void handleNewCon(qg_fd_t);

  void setMessageCallback(MessageCallBack);

  void setWriteCompleteCallBack(WriteCompleteCallBack);

  void setConnectionCallBack(ConnectionCallBack);

private:
  event_loop_pt event_loop_;
  accepter_pt accepter_;
  mp_t connections_;
  MessageCallBack message_call_back_;
  WriteCompleteCallBack write_complete_call_back_;
  ConnectionCallBack connection_call_back_;
};

} // namespace qg

#endif // SRC_ACCPETOR_H

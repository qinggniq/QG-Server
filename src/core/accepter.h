//
// Created by wc on 5/7/19.
//

#ifndef SRC_CORE_ACCEPTER_H
#define SRC_CORE_ACCEPTER_H

#include "../util/noncopyable.h"
#include "../util/type.h"
#include <functional>
#include <memory>

namespace qg {

class EventLoop;

class Socket;

class EventHandler;

class Config;

class server;

class Accepter : public qg::noncopyable {
public:
  typedef EventLoop *event_loop_pt;
  typedef EventHandler *event_handler_pt;
  typedef Socket *socket_pt;
  typedef Config *config_pt;
  typedef std::function<void(std::unique_ptr<Socket>)> ConnectionCallBack;

  explicit Accepter(event_loop_pt el, config_pt config);

  void init();

  void handleRead();

  void setNewConnectionCallBack(ConnectionCallBack);

  void incConnection() {connection_number_++;}

  void decConnection() {connection_number_--;}

private:
  config_pt config_;
  event_loop_pt event_loop_;
  socket_pt socket_;
  event_handler_pt handler_;
  ConnectionCallBack connection_call_back_;
  const int connection_high_water_;
  int connection_number_;
};

} // namespace qg

#endif // SRC_CORE_ACCEPTER_H_

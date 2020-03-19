//
// Created by wc on 5/7/19.
//

#include "accepter.h"
#include "../util/config.h"
#include "./event_handler.h"
#include "./event_loop.h"
#include "./socket.h"
#include "./tcp_connection.h"

#include <functional>

namespace qg {

/*
 * base on the initialize order
 */
Accepter::Accepter(event_loop_pt el, config_pt config)
    : event_loop_(el), config_(config),
      socket_(new Socket(config_->detail.back_log)),
      handler_(new EventHandler(socket_->sfd())) {
  handler_->SetReadCallBack(std::bind(&Accepter::handleRead, this));
  event_loop_->registerHandler(handler_);
}

void Accepter::run() {
  socket_->makeNonBlock();
  socket_->makeCloseExe();
  if (config_->detail.cork) {
    socket_->makeCork();
  }
  if (config_->keep_alive) {
    socket_->makeKeepAlive();
  }

  handler_->SetReadCallBack(std::bind(&Accepter::handleRead, this));
  event_loop_->registerHandler(handler_);
  socket_->bindAndListen(config_->port);
}

void Accepter::setNewConnectionCallBack(Accepter::ConnectionCallBack cb) {
  this->connection_call_back_ = cb;
}

//对于连接其来说，它的职责也可以被自定义。
void Accepter::handleRead() {
  qg_fd_t fd_ = socket_->accept();
  if (fd_ < 0) {
    // log
    return;
  } else {
    // TODO(qinggniq): 这里可能需要加入最大连接数限制。。
    // 创建一个TCP Connection
    connection_call_back_(fd_);
  }
}
} // namespace qg
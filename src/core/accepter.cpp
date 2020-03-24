//
// Created by wc on 5/7/19.
//

#include "./accepter.h"
#include <glog/logging.h>
#include <functional>
#include "../util/config.h"
#include "./event_handler.h"
#include "./event_loop.h"
#include "./socket.h"


namespace qg {

/*
 * base on the initialize order
 */
Accepter::Accepter(event_loop_pt el, config_pt config)
    : event_loop_(el), config_(config), socket_(new Socket()),
      handler_(new EventHandler(el, socket_->sfd())) {
  LOG(INFO) << "Acceptor construct start";
  run();
  LOG(INFO) << "Acceptor construct end";
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
  handler_->enableRead();
  socket_->bindAndListen(config_->port, config_->detail.back_log);
}

void Accepter::setNewConnectionCallBack(Accepter::ConnectionCallBack cb) {
  this->connection_call_back_ = std::move(cb);
}

//对于连接其来说，它的职责也可以被自定义。
void Accepter::handleRead() {
  qg_fd_t fd_ = socket_->accept();
  std::unique_ptr<Socket> socket(new Socket(fd_));
  if (fd_ < 0) {
    return;
  } else {
    // TODO(qinggniq): 这里可能需要加入最大连接数限制。。
    LOG(INFO) << "recv new connection on fd : " << fd_;
    connection_call_back_(std::move(socket));
  }
}
} // namespace qg
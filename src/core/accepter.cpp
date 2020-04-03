//
// Created by wc on 5/7/19.
//

#include "./accepter.h"
#include "../util/config.h"
#include "./event_handler.h"
#include "./event_loop.h"
#include "./socket.h"
#include <functional>
#include <glog/logging.h>

namespace qg {

static const  int defaultConnectionHighWater = 2000;
/*
 * base on the initialize order
 */
Accepter::Accepter(event_loop_pt el, config_pt config)
    : event_loop_(el), config_(config), socket_(new Socket()),
      handler_(new EventHandler(el, socket_->sfd())), connection_number_(0),
      connection_high_water_(defaultConnectionHighWater >
                                     config->max_connections - 10
                                 ? defaultConnectionHighWater
                                 : config->max_connections - 10) {
  LOG(INFO) << "Acceptor construct start";
  init();
  LOG(INFO) << "Acceptor construct end";
}

// 需要通过配置来确定Accepter的属性
void Accepter::init() {
  socket_->makeNonBlock();
  socket_->makeCloseExe();
  if (config_->detail.cork) {
    socket_->makeCork();
  }
  if (config_->keep_alive) {
    socket_->makeKeepAlive();
  }
  socket_->makePortReuseable();
  socket_->makeAddrReuseable();
  handler_->setReadCallBack(std::bind(&Accepter::handleRead, this));
  event_loop_->registerHandler(handler_);
  handler_->enableRead();
  // TODO(qinggniq): Accpeter 如何处理error
  socket_->bindAndListen(config_->port, config_->detail.back_log);
}

void Accepter::setNewConnectionCallBack(Accepter::ConnectionCallBack cb) {
  this->connection_call_back_ = std::move(cb);
}

void Accepter::handleRead() {
  qg_fd_t fd;
  while ((fd = socket_->accept()) > 0) {
    if (connection_number_ > connection_high_water_) {
      LOG(INFO) << "connection reach threshold, deny new connection";
      close(fd);
      break;
    } else {
      LOG(INFO) << "recv new connection on fd : " << fd;
      std::unique_ptr<Socket> socket = std::make_unique<Socket>(fd);
      connection_call_back_(std::move(socket));
    }
  }
}
} // namespace qg
//
// Created by wc on 5/7/19.
//
#include "server.h"
#include "./event_loop.h"
#include "../util/config.h"
#include "./accepter.h"
#include "./tcp_connection.h"
#include "./socket.h"
#include <memory>
#include <glog/logging.h>

namespace qg {

/*
 * base on the initialize order
 */
Server::Server(config_pt config) : event_loop_(new EventLoop()) {
  this->accepter_ = new Accepter(event_loop_, config);
  this->accepter_->setNewConnectionCallBack(std::bind(&Server::handleNewCon, this, std::placeholders::_1));
  LOG(INFO) << "server register" ;
}

Server::~Server() {
  delete event_loop_;
}

void Server::run() {
  this->event_loop_->loop();
}



void Server::handleNewCon(std::unique_ptr<Socket> socket) {
  LOG(INFO) << "fd " << socket->sfd();
  qg_fd_t fd = socket->sfd();
  socket->makeNonBlock();
  std::shared_ptr<TcpConnection> conn(new TcpConnection(this->event_loop_, std::move(socket)));
  // 设置用户设置的一些回调
  // 连接回调、消息到达回调、消息发送回调
  connections_[fd] =  conn;
  conn->setMessageCallBack(message_call_back_);
  conn->setWriteCompleteCallBack(write_complete_call_back_);
  conn->setConnectionCallBack(connection_call_back_);
  conn->handleConnection();

  LOG(INFO) << "connection end, fd : " << fd;
}
} // namespace qg
//
// Created by wc on 5/7/19.
//
#include "server.h"
#include "../util/config.h"
#include "./accepter.h"
#include "./event_loop.h"
#include "./socket.h"
#include "./tcp_connection.h"
#include <glog/logging.h>
#include <memory>

namespace qg {

/*
 * base on the initialize order
 */
Server::Server(config_pt config) : event_loop_(new EventLoop()) {
  this->accepter_ = new Accepter(event_loop_, config);
  this->accepter_->setNewConnectionCallBack(
      std::bind(&Server::handleNewCon, this, std::placeholders::_1));
  LOG(INFO) << "server register";
}

Server::~Server() { delete event_loop_; }

void Server::run() { this->event_loop_->loop(); }

void Server::handleConnectionClose(std::shared_ptr<TcpConnection> conn) {
  LOG(INFO) << "close " << conn->fd();
  connections_.erase(conn->fd());
  if (connection_close_call_back_) {
    connection_close_call_back_(conn);
  }
}

void Server::handleNewCon(std::unique_ptr<Socket> socket) {
  LOG(INFO) << "new connection coming : " << socket->sfd();
  qg_fd_t fd = socket->sfd();
  socket->makeNonBlock();
  std::shared_ptr<TcpConnection> conn(
      new TcpConnection(this->event_loop_, std::move(socket), accepter_));
  // 设置用户设置的一些回调
  // 连接回调、消息到达回调、消息发送回调
  connections_[fd] = conn;
  conn->setMessageCallBack(message_call_back_);
  conn->setWriteCompleteCallBack(write_complete_call_back_);
  conn->setConnectionComeCallBack(connection_come_call_back_);
  conn->setConnectionCloseCallBack(
      std::bind(&Server::handleConnectionClose, this, std::placeholders::_1));
  conn->handleConnection();
}
} // namespace qg
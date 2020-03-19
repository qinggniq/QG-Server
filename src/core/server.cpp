//
// Created by wc on 5/7/19.
//
#include "server.h"
#include "./event_loop.h"
#include "../util/config.h"
#include "./accepter.h"
#include "./tcp_connection.h"

namespace qg {

/*
 * base on the initialize order
 */
Server::Server(event_loop_pt event_loop, config_pt config) : event_loop_(event_loop) {
  this->accepter_ = new Accepter(event_loop_, config);
  this->accepter_->setNewConnectionCallBack(std::bind(&Server::handleNewCon, this, std::placeholders::_1));
}

void Server::run() {
  this->event_loop_->loop();
}

void Server::handleNewCon(qg_fd_t fd) {
  TcpConnection* conn = new TcpConnection(this->event_loop_, fd);
  // 设置用户设置的一些回调
  // 连接回调、消息到达回调、消息发送回调
  conn->setMessageCallBack(message_call_back_);
  conn->setWriteCompleteCallBack(write_complete_call_back_);
  conn->handleConnection();
}
} // namespace qg
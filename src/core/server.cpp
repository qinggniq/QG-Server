//
// Created by wc on 5/7/19.
//
#include "server.h"
#include "../util/config.h"
#include "./accepter.h"
#include "./event_loop.h"
#include "./socket.h"
#include "./tcp_connection.h"
#include "loop_thread_pool.h"
#include <glog/logging.h>
#include <memory>

namespace qg {

/*
 * base on the initialize order
 */
int Server::thread_num = 4;

Server::Server(config_pt config)
    : event_loop_(new EventLoop()),
      loop_pool_(new LoopThreadPool(event_loop_, config->thread_num)) {
  this->accepter_ = new Accepter(event_loop_, config);
  this->accepter_->setNewConnectionCallBack(
      std::bind(&Server::handleNewCon, this, std::placeholders::_1));
  LOG(INFO) << "server register";
}

Server::~Server() { delete event_loop_; }

void Server::run() {
  loop_pool_->start();
  this->event_loop_->loop();
}

void Server::handleConnectionClose(std::shared_ptr<TcpConnection> conn) {
  LOG(INFO) << "close " << conn->fd();
  connections_.erase(conn->fd());
  auto io_loop = conn->getLoop();
  io_loop->addFunctor(std::bind(&TcpConnection::closeConnection, conn));
}

void Server::closeAtBaseLoop(std::shared_ptr<TcpConnection> conn) {
  event_loop_->addFunctor(
      std::bind(&Server::handleConnectionClose, this, conn));
}

void Server::handleNewCon(std::unique_ptr<Socket> socket) {
  LOG(INFO) << "new connection coming : " << socket->sfd();
  qg_fd_t fd = socket->sfd();
  socket->makeNonBlock();

  EventLoop *io_loop = loop_pool_->getNextLoop();

  std::shared_ptr<TcpConnection> conn(
      new TcpConnection(io_loop, std::move(socket), accepter_));
  // 设置用户设置的一些回调
  // 连接回调、消息到达回调、消息发送回调
  connections_[fd] = conn;
  conn->setMessageCallBack(message_call_back_);
  conn->setWriteCompleteCallBack(write_complete_call_back_);
  conn->setConnectionComeCallBack(connection_come_call_back_);
  conn->setConnectionCloseCallBack(
      std::bind(&Server::closeAtBaseLoop, this, std::placeholders::_1));
  // 这个任务应该放到Loop里面执行
  io_loop->addFunctor(std::bind(&TcpConnection::handleConnection, conn));
}
} // namespace qg
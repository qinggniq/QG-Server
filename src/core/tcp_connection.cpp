//
// Created by wc on 5/6/19.
//

#include "tcp_connection.h"
#include "../util/rwtool.h"
#include "event_handler.h"
#include "event_loop.h"
#include "socket.h"
#include <stdio.h>
#include <string>

using namespace qg;
int TcpConnection::defaultReadBuffSize = 1024;

// TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, qg_fd_t fd)
    : TcpConnection::TcpConnection(el, new Socket(fd)) {}

// TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, socket_pt sock)
    : event_loop_(el), socket_(sock) {
  //初始化读缓存，写缓存
  read_buf_ = new std::string;
  write_buf_ = new std::string;
  event_handler_ = new EventHandler(socket_->sfd());
  event_handler_->SetReadCallBack(
      std::bind(&TcpConnection::handleRead, this));
  event_handler_->SetWriteCallBack(
      std::bind(&TcpConnection::handleWrite, this));
  event_loop_->registerHandler(event_handler_);
}

//这个函数在客户端第一次连接的时候调用
void TcpConnection::handleConnection() {
  ::printf(socket_->Info().c_str());
}

void TcpConnection::handleRead() {
  // TODO: 这里要处理一下细节，不一定一旦有数据就要读
  std::string buf;
  while (QGReadN(socket_->sfd(), buf, TcpConnection::defaultReadBuffSize) > 0) {
    *read_buf_ += buf;
  }
  // assert( != nullptr);
  // TODO: 这里可能需要交给线程池去执行业务代码
  // 这里不是线程安全的，
  message_call_back_(shared_from_this(), read_buf_);
  read_buf_->clear();
}

// 这个函数给业务代码写要发送给客户端的消息
// 它不会真正的调用handleWrite
void TcpConnection::write(buf_pt str) {
  // 连接不一定可以写，但是业务代码不应该知道这点
  if (event_handler_->IsWriteable()) {
    // 先直接append，可能会有线程安全问题
    write_buf_->append(*str);
  }
}

void TcpConnection::handleWrite() {
  // TcpConnection
  if (write_buf_->empty())
    return;
  assert(event_handler_->IsWriteable());
  int n = QGWriteN(socket_->sfd(), *write_buf_, write_buf_->size());
  // BUG:
  // 这里有问题，如果内核缓冲区剩余大小小于缓冲数据大小，这里直接clear就会数据丢失。
  // 需要记录写多少字符，然后
  if (n >= 0) {
    *write_buf_ = write_buf_->substr(n);
    if (write_buf_->empty()) {
      write_complete_call_back_(shared_from_this(), write_buf_);
    }
  } else {
    // 一般就是记录。
  }
}
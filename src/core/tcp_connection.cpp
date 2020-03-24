//
// Created by wc on 5/6/19.
//

#include "tcp_connection.h"
#include "../util/rwtool.h"
#include "event_handler.h"
#include "event_loop.h"
#include "socket.h"
#include <glog/logging.h>
#include <string>
using namespace qg;
int TcpConnection::defaultReadBuffSize = 1024;

// TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, std::unique_ptr<Socket> socket)
    : event_loop_(el), socket_(std::move(socket)),
      event_handler_(new EventHandler(el, socket_->sfd())) {
  //   初始化读缓存，写缓存
  read_buf_ = new std::string;
  write_buf_ = new std::string;
  // TODO(qinggniq): remove
  event_handler_->SetReadCallBack(std::bind(&TcpConnection::handleRead, this));
  event_handler_->SetWriteCallBack(
      std::bind(&TcpConnection::handleWrite, this));
  event_loop_->registerHandler(event_handler_);
  event_handler_->disableWrite();
}

TcpConnection::~TcpConnection() {
  // 析构的时候要仔细考虑
  delete read_buf_;
  delete write_buf_;
  event_handler_->disableAll();
}

// 这个函数在客户端第一次连接的时候调用
void TcpConnection::handleConnection() {
  LOG(INFO) << socket_->sfd();
  event_handler_->enableRead();
  connection_call_back_();
}

void TcpConnection::handleRead() {
  LOG(INFO) << "handle read";
  std::string buf;
  int cnt = 0, n = 0;
  while ((n = QGReadN(socket_->sfd(), buf,
                      TcpConnection::defaultReadBuffSize)) > 0) {
    cnt += n;
    *read_buf_ += buf;
  }
  LOG(INFO) << "read n : " << cnt << " byte";
  if (cnt > 0) {
    message_call_back_(shared_from_this(), read_buf_);
  } else if (cnt == 0) {
    // TODO(qinggniq): 析构逻辑
    // handleClose();
    event_handler_->disableAll();
  } else {
    LOG(ERROR) << "error in TcpConnection::handleRead";
    // TODO(qinggniq):
    // handleErr();
  }
  read_buf_->clear();
  LOG(INFO) << "handle end";
}

// 这个函数给业务代码写要发送给客户端的消息
// 它不会真正的调用handleWrite
void TcpConnection::write(buf_pt data) {
  // 如果连接没有注册写事件回调，那么就直接写
  LOG(INFO) << "write data : " << *data;
  int len = data->size();
  int remain = len, nwrite = 0;
  if (!event_handler_->isWriting() && write_buf_->empty()) {
    LOG(INFO) << "here";
    // 先直接append，可能会有线程安全问题
    nwrite = QGWriteN(socket_->sfd(), *data, len);
    if (nwrite > 0) {
      if (nwrite == len) {
        write_complete_call_back_(shared_from_this(), write_buf_);
      }
    }
    remain = len - nwrite;
  }
  assert(remain <= data->size());
  if (remain > 0) {
    *write_buf_ += data->substr(nwrite);
    if (!event_handler_->isWriting()) {
      event_handler_->enableWrite();
    }
  }
}

void TcpConnection::handleWrite() {
  LOG(INFO) << "handle write";
  // TcpConnection
  if (write_buf_->empty())
    return;
  assert(event_handler_->isWriteable());
  int nwrite = QGWriteN(socket_->sfd(), *write_buf_, write_buf_->size());

  if (nwrite >= 0) {
    *write_buf_ = write_buf_->substr(nwrite);
    if (write_buf_->empty()) {
      write_complete_call_back_(shared_from_this(), write_buf_);
    }
  } else {
  }
}
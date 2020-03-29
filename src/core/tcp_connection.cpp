//
// Created by wc on 5/6/19.
//
#include "./tcp_connection.h"
#include "./accepter.h"
#include "../util/rwtool.h"
#include "event_handler.h"
#include "event_loop.h"
#include "socket.h"
#include <glog/logging.h>

using namespace qg;
int TcpConnection::defaultReadBuffSize = 1024;

// TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, std::unique_ptr<Socket> socket, Accepter* accepter)
    : event_loop_(el), socket_(std::move(socket)),
      event_handler_(new EventHandler(el, socket_->sfd())),
      accpeter_(accepter),
      disconnected_(false) {
  read_buf_ = new std::string;
  write_buf_ = new std::string;
  accpeter_->incConnection();
  event_handler_->SetReadCallBack(std::bind(&TcpConnection::handleRead, this));
  event_handler_->SetWriteCallBack(
      std::bind(&TcpConnection::handleWrite, this));
  event_loop_->registerHandler(event_handler_);
}
// 析构的时候要仔细考虑
TcpConnection::~TcpConnection() {
  LOG(INFO) << "[Connection Closed]" << fd();
  delete read_buf_;
  delete write_buf_;
  accpeter_->decConnection();
  event_handler_->disableAll();
  event_loop_->removeHandler(event_handler_);
  LOG(INFO) << "[Connection Closed] end";
}

// 这个函数在客户端第一次连接的时候调用
void TcpConnection::handleConnection() {
  LOG(INFO) << "[Connection] " << socket_->Info();
  event_handler_->enableRead();
  if (connection_come_call_back_) {
    connection_come_call_back_();
  }
}

// 只有可以非阻塞读的时候handleRead才会触发。
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
    read_buf_->clear();
  } else if (cnt == 0) {
    // TODO(qinggniq): 读到0说明客户端已经关闭了连接，
    //  这个时候服务端需要将write_buf里面的数据发送完了再去关闭连接
    disconnected_ = true;
    if (write_buf_->empty()) {
      handleClose();
    } else {
      handleWrite();
    }
  } else {
    LOG(ERROR) << "error in TcpConnection::handleRead";
    disconnected_ = true;
    handleClose();
  }

  LOG(INFO) << "handle end";
}

// 如果在write的过程中客户端关闭了连接，那么write只能调用一次（合理吗，合理）。
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
      if (disconnected_) {
        handleClose();
        return;
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
  if (write_buf_->empty())
    return;
  assert(event_handler_->isWriteable());
  int nwrite = QGWriteN(socket_->sfd(), *write_buf_, write_buf_->size());

  if (nwrite >= 0) {
    *write_buf_ = write_buf_->substr(nwrite);
    if (write_buf_->empty()) {
      event_handler_->disableWrite();
      write_complete_call_back_(shared_from_this(), write_buf_);
    }
    // 这里要判断一下对端的状态，如果对端已经断开连接了，这样可能会发生sigpipe。
  } else {
    LOG(ERROR) << "Tcp::handleWrite Error";
  }
  if (disconnected_) {
    handleClose();
  }
}

// Close 很关键，如果TcpConnection在handle
// Event（多线程）的时候被析构了，那么就会core dump。
void TcpConnection::handleClose() {
  LOG(INFO) << "close fd : " << socket_->Info();
  assert(disconnected_);
  if (connection_close_call_back_) {
    // 在这里去除 server 对connection 的引用会使TcpConnection立刻析构，
    //std::shared_ptr<TcpConnection> objectGurd(shared_from_this());
    connection_close_call_back_(shared_from_this());
  }
}
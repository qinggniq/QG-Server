//
// Created by wc on 5/6/19.
//
#include "./tcp_connection.h"
#include "../util/rwtool.h"
#include "./accepter.h"
#include "event_handler.h"
#include "event_loop.h"
#include "socket.h"
#include <glog/logging.h>
#ifdef __linux__
#include <sys/sendfile.h>
#else
#include <fstream>
#endif

using namespace qg;
int TcpConnection::defaultReadBuffSize = 1024;
int TcpConnection::kMaxFileSize = 10 * 1024 * 1024;

// TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, std::unique_ptr<Socket> socket,
                             Accepter *accepter)
    : event_loop_(el), socket_(std::move(socket)),
      event_handler_(new EventHandler(el, socket_->sfd())), accpeter_(accepter),
      disconnected_(false), sendfile_fd_(-1) {
  accpeter_->incConnection();
  event_handler_->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
  event_handler_->setWriteCallBack(
      std::bind(&TcpConnection::handleWrite, this));
  event_loop_->registerHandler(event_handler_);
  LOG(INFO) << "connection come at " << event_loop_ ;
}
// 析构的时候要仔细考虑
TcpConnection::~TcpConnection() {
  LOG(INFO) << "[Connection Closed]" << fd();
  write_buf_.clear();
  read_buf_.clear();
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
    connection_come_call_back_(shared_from_this());
  }
}

// 只有可以非阻塞读的时候handleRead才会触发。
void TcpConnection::handleRead() {
  LOG(INFO) << "handle read";
  std::string buf;
  int cnt = 0, n = 0;
  LOG(INFO) << socket_->sfd() << " " << buf << " ";
  while ((n = qg::read(socket_->sfd(), buf, TcpConnection::defaultReadBuffSize)) > 0) {
    cnt += n;
    read_buf_ += buf;
  }
  LOG(INFO) << "read n : " << cnt << " byte";
  if (cnt > 0) {
    message_call_back_(shared_from_this(), read_buf_);
    read_buf_.clear();
  } else if (cnt == 0) {
    // TODO(qinggniq): 读到0说明客户端已经关闭了连接，
    //  这个时候服务端需要将write_buf里面的数据发送完了再去关闭连接
    close();
  } else {
    LOG(ERROR) << "error in TcpConnection::handleRead";
    disconnected_ = true;
    handleClose();
  }
}

// 如果在write的过程中客户端关闭了连接，那么write只能调用一次（合理吗，合理）。
void TcpConnection::write(const buf_t& data) {
  // 如果连接没有注册写事件回调，那么就直接写
  LOG(INFO) << "write data : " << data;
  int len = data.size();
  int remain = len, nwrite = 0;
  // 开始的时候并不关注写事件，并且buffer为空
  bool canWriteDirect = !event_handler_->isWriting() && write_buf_.empty();
  LOG(INFO) << "can wirte ? : " << canWriteDirect;
  if (canWriteDirect) {
    nwrite = qg::write(socket_->sfd(), data.c_str(), len);
    if (nwrite > 0) {
      if (nwrite == len && sendfile_fd_ < 0) {
        write_complete_call_back_(shared_from_this(), write_buf_);
      }
      if (disconnected_) {
        handleClose();
        return;
      }
    }
  } else if (sendfile_fd_ >= 0) {
#ifdef __linux__
    ssize_t ret = ::sendfile(socket_->sfd(), sendfile_fd_, NULL, 16384);
    while (ret >= 0) {
      if (ret == 0) {
        ::close(sendfile_fd);
        sendfile_fd = -1;
        writeCompleteCallBack(share_from_this);
        break;
      }
      ret = ::sendfile(socket->get_fd(), sendfile_fd, NULL, 16384);
    }
    if (ret < 0 && ret != EAGAIN) {
      syscall_error();
    }
#endif
  }
  write_buf_ += data.substr(nwrite);
  if (hasContentToSend()) {
    if (!event_handler_->isWriting()) {
      event_handler_->enableWrite();
    }
  }
}

void TcpConnection::handleWrite() {
  LOG(INFO) << "handle write";
  if (write_buf_.empty())
    return;
  int nwrite = qg::write(socket_->sfd(), write_buf_.c_str(), write_buf_.size());
  LOG(INFO) << "here";
  if (nwrite >= 0) {
    write_buf_ = write_buf_.substr(nwrite);
    if (write_buf_.empty()) {
      event_handler_->disableWrite();
      write_complete_call_back_(shared_from_this(), write_buf_);
    }
    LOG(INFO) << "write_complete_call_back";
    // 这里要判断一下对端的状态，如果对端已经断开连接了，这样可能会发生sigpipe。
  } else {
    LOG(ERROR) << "Tcp::handleWrite Error";
    disconnected_ = true;
    handleClose();
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
    std::shared_ptr<TcpConnection> objectGurd(shared_from_this());
    connection_close_call_back_(shared_from_this());
  }
}

void TcpConnection::sendFile(const qg_string &file_name) {
#ifdef __linux__
  assert(0 && "not implement");
#else
//
//  write_buf_.append();
//  event_handler_->enableWrite();
//  handleWrite();
#endif
}

void TcpConnection::close()  {
  disconnected_ = true;
  if (hasContentToSend()) {
    LOG(INFO) << "has content to send";
  }else{
    handleClose();
  }
}
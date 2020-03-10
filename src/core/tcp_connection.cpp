//
// Created by wc on 5/6/19.
//

#include <stdio.h>
#include "tcp_connection.h"
#include "socket.h"
#include "event_handler.h"
#include "event_loop.h"
#include "../util/rwtool.h"

using namespace qg;
int TcpConnection::defaultReadBuffSize = 1024;

//TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, qg_fd_t fd) : TcpConnection::TcpConnection(el, new Socket(fd)) {
}

//TcpConnection 的控制权交给Accepter
TcpConnection::TcpConnection(event_loop_pt el, socket_pt sock) : event_loop_(el), socket_(sock) {
    //初始化读缓存，写缓存
    event_handler_ = new EventHandler(socket_->sfd());
    event_handler_->SetReadCallBack(std::bind(&TcpConnection::handleRead, this));
    event_handler_->SetWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
}

//这个函数在客户端第一次连接的时候调用
void TcpConnection::handleConnection() {
    ::printf(socket_->Info().c_str());
}


void TcpConnection::handleRead() {
    //TODO: 这里要处理一下细节，不一定一旦有数据就要读
    std::string buf;
    while (QGReadN(socket_->sfd(), buf, TcpConnection::defaultReadBuffSize) > 0) {
        *read_buf_ += buf;
    }
    assert(read_cb_ != nullptr);
    //TODO: 这里可能需要交给线程池去执行业务代码
    //这里不是线程安全的，
    read_cb_(this, read_buf_, write_buf_);
    read_buf_->clear();
}

//这个函数给业务代码写要发送给客户端的消息
//它不会真正的调用handleWrite
void TcpConnection::write(buf_pt str) {
    //连接不一定可以写，但是业务代码不应该知道这点
    if (event_handler_->IsWriteable()) {
        //先直接append，可能会有线程安全问题
        write_buf_->append(*str);
    }
}

void TcpConnection::handleWrite() {
    //TcpConnection
    if (write_buf_->empty()) return;
    assert(event_handler_->IsWriteable());
    QGWriteN(socket_->sfd(), *write_buf_, write_buf_->size());
    write_buf_->clear();
}
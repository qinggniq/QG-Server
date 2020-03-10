//
// Created by wc on 5/7/19.
//
#include "accepter.h"
#include <functional>
#include <unistd.h>
#include "../util/config.h"
#include "socket.h"
#include "event_handler.h"
#include "event_loop.h"
#include "tcp_connection.h"


namespace qg {


/*
 * base on the initialize order
 */
    Accepter::Accepter(event_loop_pt el,
                       config_pt config) :
            event_loop_(el),
            config_(config),
            socket_(new Socket(config_->detail.back_log)),
            handler_(new EventHandler(socket_->sfd())) {
        handler_->SetReadCallBack(std::bind(&Accepter::handleNewCon, this));
        event_loop_->registerHandler(handler_);
    }

    void Accepter::run() {
        socket_->makeNonBlock();
        socket_->makeCloseExe();
        if (config_->detail.cork) {
            socket_->makeCork();
        }
        if (config_->keep_alive) {
            socket_->makeKeepAlive();
        }

        handler_->SetReadCallBack(std::bind(&Accepter::handleNewCon, this));
        event_loop_->registerHandler(handler_);
        socket_->bindAndListen(config_->port);
    }

    //对于连接其来说，它的职责也可以被自定义。
    void Accepter::handleNewCon() {
        Socket *child_socket = socket_->accept();
        if (child_socket == nullptr) return;
        else {
            //TODO: 这里可能需要加入最大连接数限制。。
            //创建一个TCP Connection
            TcpConnection *conn = new TcpConnection(event_loop_, child_socket);
            //这里调用Connection的handleConnection
            conn->handleConnection();
            event_loop_->registerHandler(conn->eventHandler());
        }
    }
} //namespace qg
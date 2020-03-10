//
// Created by wc on 5/7/19.
//
#include "Server.h"

#include <unistd.h>
#include "../util/config.h"
#include "socket.h"
#include "event_handler.h"
#include "event_loop.h"


namespace qg {


/*
 * base on the initialize order
 */
    Server::Server(event_loop_pt el,
                   config_pt config) :
            event_loop_(el),
            config_(config),
            socket_(new Socket(config_->detail.back_log)),
            handler_(new EventHandler( socket_->sfd())) {

    }

    void Server::run() {
        Socket::makeAddrReuseable(socket_->sfd());
        Socket::makeNonBlock(socket_->sfd());
        Socket::makeCloseExe(socket_->sfd());
        if (config_->detail.cork) {
            Socket::makeCork(socket_->sfd());
        }
        if (config_->keep_alive) {
            Socket::makeKeepAlive(socket_->sfd());
        }
        handler_->SetReadCallBack(std::bind(&Server::handleNewCon, this));
        event_loop_->registerHandler(handler_);
        socket_->bindAndListen(config_->port);
    }


    void Server::handleNewCon() {
        int cli_fd = 0;
        while ((cli_fd = socket_->accept()) > 0) {
            if (cli_fd >= config_->max_connections) {
                ::close(cli_fd);
                continue;
            }
        }

    }


} //namespace qg
//
// Created by wc on 5/7/19.
//
#include "Server.h"

#include <netinet/in.h>
namespace qg{


/*
 * base on the initialize order
 */
Server::Server( dispatcher_pt dispatcher,
				config_pt config):
				dispatcher_(std::move(dispatcher)),
				config_(std::move(config)),
				running_(false),
				socket_(new Socket(config_->detail.back_log)),
				handler_(new EventHandler(dispatcher_, socket_->sfd())){


}

void Server::run() {
  socket_->makeNonBlock();
  socket_->makeCloseExe();
  if (config_->detail.cork) {
    socket_->makeCork();
  }
  if (config_->keep_alive) {
    socket_->makeKeepAlive();
  }

  handler_->SetReadCallBack(std::bind(&Server::handleNewCon, this));
  dispatcher_->registerHandler(handler_);
  socket_->bindAndListen(config_->port);
  running_ = true;
}


void Server::handleNewCon() {
  int cli_fd = 0;
  while ((cli_fd = socket_->accept()) > 0) {
	if (cli_fd >= config_->max_connections) {
	  close(cli_fd);
	  continue;
	}if (cli_fd >= config_->max_connections) {
	  close(cli_fd);
	  continue;
	}


  }

}



} //namespace qg
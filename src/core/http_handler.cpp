//
// Created by wc on 5/7/19.
//

#include "http_handler.h"
namespace qg{
HttpHandler::HttpHandler(dispatcher_pt dispatcher,
	                     qg_fd_t fd) :
	                     dispatcher_(std::move(dispatcher)),
	                     fd_(fd),
	                     request_data_(new RequestData()),
	                     request_parser_(new RequestParser(request_data_)),
						 handler_(new EventHandler(dispatcher, fd)){

  handler_->enableAll();
  handler_->SetReadCallBack(std::bind(&HttpHandler::handleRead, this));
  handler_->SetWriteCallBack(std::bind(&HttpHandler::handleWrite, this));
  handler_->SetErrorCallBack(std::bind(&HttpHandler::handleError, this));
  dispatcher->registerHandler(handler_);
}

void HttpHandler::handleRead() {

}

void HttpHandler::handleWrite() {
  ResponseData red;

}

void HttpHandler::handleError() {
  ResponseData red;
}
						 
} //namespace qg
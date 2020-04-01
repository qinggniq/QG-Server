//
// Created by 王聪 on 2020/4/1.
//

#include "http_server.h"
#include "../server.h"
#include "./http_context.h"
#include "./http_request.h"
#include "./http_response.h"
#include <any>
#include <glog/logging.h>
namespace qg {
HTTPServer::HTTPServer(Config *config) : server_(new Server(config)) {
  server_->setMessageCallback(std::bind(&HTTPServer::handleMessageCome, this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
  server_->setConnectionComeCallBack(std::bind(
      &HTTPServer::handleConnectionCome, this, std::placeholders::_1));
  request_call_back_ = std::move(std::bind(&HTTPServer::defaultHandleRequest, this, std::placeholders::_1, std::placeholders::_2));
}

void HTTPServer::handleMessageCome(std::shared_ptr<TcpConnection> conn,
                                   buf_pt read_buf) {
  auto http_context =
      std::any_cast<std::shared_ptr<HTTPContext>>(conn->context());
  auto setting = http_context->getSettings();
  auto parser = http_context->getParser();
  auto request = http_context->getRequest();
  int nparse =
      http_parser_execute(parser, setting, read_buf->c_str(), read_buf->size());
  if (nparse != read_buf->size()) {
    LOG(ERROR) << "ERROR when parse http request";
    // FIXME(qinggniq) 这里可能有析构的风险
    conn->shutdown();
    return;
  }
  if (HTTP_PARSER_ERRNO(parser) != HPE_OK) {
    // TODO(qinggniq) 返回一个bad request
    // conn->write("")
    LOG(ERROR) << "Client send a bad request "
               << http_errno_name(HTTP_PARSER_ERRNO(parser));
    conn->shutdown();
    return;
  }
  if (request->body_is_final) {
    request->printRequest();
    // TODO(qinggniq):
    // 构造response，然后清空request。在handleWrite的时候同时来了一个request怎么办。
    // FIXME(qinggniq):
    // 暂时定的是，先弄一个新的http_request,然后swap，然后处理http_request,
    // 这个时候其他的 线程使用这个http_requst的话也不会崩溃。
    // FIXME(qinggni): 但是一个Connection里面的东西处理事件只在一个线程。
    // 初始化的时候可能用read_buf去初始化比较好。。。。
    auto response = new HTTPResponse();
    request_call_back_(request, response);
    qg_string s = std::move(response->toString());
    conn->write(&s);
    http_context->reset();
    // 这里的析构要好好考虑
    delete (request);
    delete (response);
  }
}

void HTTPServer::handleWriteComplete(std::shared_ptr<TcpConnection> conn,
                                      buf_pt read_buf) {
  auto http_context =
      std::any_cast<std::shared_ptr<HTTPContext>>(conn->context());
  auto parser = http_context->getParser();
  if (!http_should_keep_alive(parser)) {
    conn->shutdown();
  }
  LOG(INFO) << "Response send success";
}

void HTTPServer::handleConnectionCome(std::shared_ptr<TcpConnection> conn) {
  auto http_context = std::make_shared<HTTPContext>();
  http_context->bindParser(http_context->getParser());
  conn->setContext(std::move(http_context));
  // 这里可能需要记录一下ip限制性？
  // data设置为request，那么http-parser的callback就只能填数据，而不能进行任何IO操作。合理。
  LOG(INFO) << "New HTTP Client Come in";
}

void HTTPServer::handleConnectionClose(std::shared_ptr<TcpConnection> conn) {
  LOG(INFO) << "HTTP Client leave";
}

void HTTPServer::defaultHandleRequest(qg::HTTPRequest *, qg::HTTPResponse *) {

}



}; // namespace qg
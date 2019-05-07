//
// Created by wc on 5/7/19.
//

#ifndef SRC_HTTP_HANDLER_H
#define SRC_HTTP_HANDLER_H
#include "http_parser.h"
#include "reactor.h"

namespace qg{

class HttpHandler{
 public:
  typedef std::shared_ptr<RequestData> request_data_pt;
  typedef std::shared_ptr<RequestParser> request_parser_pt;
  typedef std::shared_ptr<Dispatcher> dispatcher_pt;
  typedef std::shared_ptr<EventHandler> handler_pt;
  typedef std::function<void()> Functor;

  explicit HttpHandler(dispatcher_pt dispatcher, qg_fd_t fd);
  void handleRead();
  void handleWrite();
  void handleError();
 private:
  qg_fd_t fd_;
  dispatcher_pt dispatcher_;
  request_data_pt request_data_;
  request_parser_pt request_parser_;
  handler_pt handler_;
};
} //namespace qg
#endif //SRC_HTTP_HANDLER_H

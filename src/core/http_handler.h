//
// Created by wc on 5/7/19.
//

#ifndef SRC_HTTP_HANDLER_H
#define SRC_HTTP_HANDLER_H
#include "http_parser.h"
#include "reactor.h"
#include "config.h"

namespace qg{
enum ConnectionStatus {
  kDisConnected,
  kConnected
};

class HttpHandler{
 public:
  typedef std::shared_ptr<RequestData> request_data_pt;
  typedef std::shared_ptr<RequestParser> request_parser_pt;
  typedef std::shared_ptr<Dispatcher> dispatcher_pt;
  typedef std::shared_ptr<EventHandler> handler_pt;
  typedef std::shared_ptr<Config> config_pt;
  typedef qg_string response_data_pt;
  typedef std::function<void()> Functor;

  explicit HttpHandler(dispatcher_pt dispatcher, qg_fd_t fd, config_pt config);
  void handleRead();
  void handleWrite();
  void handleError(qg_int err_num, qg_string&& note);
 private:
  qg_fd_t fd_;

  dispatcher_pt dispatcher_;

  request_data_pt request_data_;
  request_parser_pt request_parser_;
  handler_pt handler_;
  response_data_pt response_;
  config_pt config_;
  ConnectionStatus conn_status_;
  qg_bool keep_alive_;
  qg_bool error_;
};
} //namespace qg
#endif //SRC_HTTP_HANDLER_H

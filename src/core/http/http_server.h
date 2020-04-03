//
// Created by 王聪 on 2020/4/1.
//

#ifndef QGSERVER_SRC_CORE_HTTP_HTTP_SERVER_H_
#define QGSERVER_SRC_CORE_HTTP_HTTP_SERVER_H_
#include "../server.h"
namespace qg {
class EventLoop;
class TcpConnection;
class HTTPRequest;
class HTTPResponse;
struct Config;
class HTTPServer {
public:
  typedef std::function<void(std::shared_ptr<HTTPRequest> &,
                             std::shared_ptr<HTTPResponse> &)>
      RequestCallBack;
  typedef std::map<int, std::map<qg_string, RequestCallBack>> router_t;
  // 可能加入Server选项，定制的httpConfig
  explicit HTTPServer(Config *config);
  enum Method { GET, PUT, DELETE, POST };
  void run() { server_->run(); }
  void route(qg_string path, Method method, RequestCallBack cb) {
    router_[method].emplace(path, std::move(cb));
  }

private:
  static int kMaxFileSize;

private:
  void handleConnectionCome(std::shared_ptr<TcpConnection> );
  void handleMessageCome(std::shared_ptr<TcpConnection> , buf_t &);
  void handleConnectionClose(std::shared_ptr<TcpConnection> );
  void handleWriteComplete(std::shared_ptr<TcpConnection> conn,
                           buf_t &read_buf);
  void handleRequest(std::shared_ptr<HTTPRequest> &,
                     std::shared_ptr<HTTPResponse> &);
  void defaultHandleRequest(std::shared_ptr<HTTPRequest> &,
                            std::shared_ptr<HTTPResponse> &);
  Server *server_;
  router_t router_;
};

}; // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_HTTP_SERVER_H_

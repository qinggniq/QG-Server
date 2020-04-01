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
  typedef std::function<void(HTTPRequest *, HTTPResponse *)> RequestCallBack;
  // 可能加入Server选项，定制的httpConfig
  explicit HTTPServer(Config *config);
  void run() { server_->run(); }
  void setRequestCallBack(RequestCallBack cb) {
    request_call_back_ = std::move(cb);
  }

private:
  void handleConnectionCome(std::shared_ptr<TcpConnection>);
  void handleMessageCome(std::shared_ptr<TcpConnection>, buf_pt);
  void handleConnectionClose(std::shared_ptr<TcpConnection>);
  void handleWriteComplete(std::shared_ptr<TcpConnection> conn,
                           buf_pt read_buf);
  void defaultHandleRequest(HTTPRequest *, HTTPResponse *);
  Server *server_;
  RequestCallBack request_call_back_;
};

}; // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_HTTP_SERVER_H_

//
// Created by 王聪 on 2020/3/29.
//

#ifndef QGSERVER_SRC_CORE_HTTP_HTTP_CONTEXT_H_
#define QGSERVER_SRC_CORE_HTTP_HTTP_CONTEXT_H_
#include "../../lib/http-parser/http_parser.h"
#include "../../util/noncopyable.h"
#include "../../util/type.h"
#include "../tcp_connection.h"
#include "./http_request.h"
#include <utility>

namespace qg {
struct HTTPRequest;
struct HTTPResponse;
class HTTPContext : public noncopyable {
public:
  typedef std::function<void(std::shared_ptr<HTTPRequest>, std::shared_ptr<HTTPResponse>)> RequestCallBack;

  HTTPContext();
  ~HTTPContext();

  void reset() {
    auto request = std::make_shared<HTTPRequest>();
    std::swap(http_request_, request);
    http_parser_init(parser_, HTTP_REQUEST);
  }

  void bindParser(struct http_parser *parser) { parser->data = this; }

  int parse(const qg_string &data);
  [[nodiscard]] struct http_parser *getParser() const { return parser_; }

  [[nodiscard]] std::shared_ptr<HTTPRequest> getRequest() const { return http_request_; }
  [[nodiscard]] struct http_parser_settings *getSettings() const {
    return settings_;
  }

private:
  struct http_parser *parser_;
  struct http_parser_settings *settings_;
  std::shared_ptr<HTTPRequest> http_request_;
};
} // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_HTTP_CONTEXT_H_

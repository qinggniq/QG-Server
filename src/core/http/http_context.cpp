//
// Created by 王聪 on 2020/3/29.
//

#include "http_context.h"
#include "http_request.h"
#include "http_response.h"
#include <iostream>
#include <typeinfo>

namespace qg {
namespace net {

typedef int (*http_data_cb)(http_parser *, const char *buf, size_t len);
typedef int (*http_cb)(http_parser *);

inline HTTPRequest *getRequest(const http_parser *parser) {
  return static_cast<HTTPContext *>(parser->data)->getRequest();
}

int isBodyFinal(const http_parser *parser) {
  auto http_request = getRequest(parser);

  if (http_request->body_is_final) {
    fprintf(stderr, "\n\n *** Error http_body_is_final() should return 1 "
                    "on last on_body callback call "
                    "but it doesn't! ***\n\n");
    assert(0);
    abort();
  }
  http_request->body_is_final = http_body_is_final(parser);
  return 0;
}

int defaultTriggerCallBack(http_parser *parser) {
  return 0;
}
int defaultMessageCallBack(http_parser *parser, const char *buf, size_t len) {
  return 0;
}

inline void fillURLSubStr(HTTPRequest *request, struct http_parser_url *u,
                          enum http_parser_url_fields field) {
  std::string *target_ptr;
  switch (field) {
  case UF_QUERY:
    target_ptr = &request->query_string;
    break;
  case UF_FRAGMENT:
    target_ptr = &request->fragment;
    break;
  case UF_PATH:
    target_ptr = &request->request_path;
    break;
  default:
    LOG(ERROR) << "unpossible condition";
    abort();
  }
  auto &url = request->request_url;
  if (u->field_set & (1 << field)) {
    target_ptr->append(url, u->field_data[field].off, u->field_data[field].len);
  }
}
// 这个URL有问题
int urlCallBack(http_parser *parser, const char *buf, size_t len) {
  HTTPRequest *http_request = getRequest(parser);

  struct http_parser_url u;
  http_request->request_url.append(buf, len);
  auto &url = http_request->request_url;
  if (http_parser_parse_url(url.c_str(), url.size(), 0, &u)) {
    LOG(ERROR) << "error when parsing url, url [ " << url << " ]";
    abort();
  }
  http_request->method = static_cast<http_method>(parser->method);
  for (auto field : {UF_QUERY, UF_FRAGMENT, UF_PATH}) {
    fillURLSubStr(http_request, &u, field);
  }
  return 0;
}

// 有可能它没有parse 完？
int headerFieldCallBack(http_parser *parser, const char *buf, size_t len) {
  auto http_request = getRequest(parser);
  if (http_request->last_header_element != HTTPRequest::FIELD) {
    if (!http_request->last_filed_value.first.empty()) {
      http_request->headers.emplace(http_request->last_filed_value.first,
                                    http_request->last_filed_value.second);
    }
    http_request->last_filed_value = std::make_pair(qg_string(), qg_string());
  }
  http_request->last_filed_value.first.append(buf, len);
  http_request->last_header_element = HTTPRequest::FIELD;
  return 0;
}

int headerValueCallBack(http_parser *parser, const char *buf, size_t len) {
  auto http_request = getRequest(parser);
  http_request->last_filed_value.second.append(buf, len);
  http_request->last_header_element = HTTPRequest::VALUE;
  return 0;
}

int bodyCallBack(http_parser *parser, const char *buf, size_t len) {
  auto http_request = getRequest(parser);
  http_request->body.append(buf, len);
  isBodyFinal(parser);
  return 0;
}

int messageCompleteCallBack(http_parser *parser) {
  auto http_request = getRequest(parser);
  if (!http_request->last_filed_value.second.empty()) {
    http_request->headers.emplace(http_request->last_filed_value);
  }
  http_request->message_is_end = true;
  return 0;
}
int headerCompleteCallBack(http_parser *parser) {
  auto http_request = getRequest(parser);
  if (!http_request->last_filed_value.second.empty()) {
    http_request->headers.emplace(http_request->last_filed_value);
  }
  return 0;
}
static struct http_parser_settings defaultParserSettig = {
    .on_message_begin = net::defaultTriggerCallBack,
    .on_header_field = net::headerFieldCallBack,
    .on_header_value = net::headerValueCallBack,
    .on_url = net::urlCallBack,
    .on_status = net::defaultMessageCallBack,
    .on_body = net::bodyCallBack,
    .on_headers_complete = net::headerCompleteCallBack,
    .on_message_complete = net::messageCompleteCallBack,
    .on_chunk_header = net::defaultTriggerCallBack,
    .on_chunk_complete = net::defaultTriggerCallBack};
} // namespace net
HTTPContext::HTTPContext() {
  parser_ =
      static_cast<struct http_parser *>(::malloc(sizeof(struct http_parser)));
  http_parser_init(parser_, HTTP_REQUEST);
  bindParser(parser_);
  settings_ = &net::defaultParserSettig;
  http_request_ = new HTTPRequest();
}

HTTPContext::~HTTPContext() { delete http_request_; }

// used for test
void HTTPContext::execute(const char *buf, int len) {
  int n = http_parser_execute(parser_, settings_, buf, len);
  if (n != len) {
    assert(0 && "有问题");
  }
  http_request_->printRequest();
  delete (http_request_);
  reset();
}
} // namespace qg

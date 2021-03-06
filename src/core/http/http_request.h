//
// Created by 王聪 on 2020/3/29.
//

#ifndef QGSERVER_SRC_CORE_HTTP_HTTP_REQUEST_H_
#define QGSERVER_SRC_CORE_HTTP_HTTP_REQUEST_H_
#include "../../lib/http-parser/http_parser.h"
#include "../../util/type.h"
#include <glog/logging.h>
#include <iostream>
#include <map>
namespace qg {
class HTTPRequest {
public:
  HTTPRequest()
      : body_is_final(false), message_is_end(false), should_keep_alive(false) {}
  ~HTTPRequest() { LOG(INFO) << "~HTTPRequest"; }
  typedef std::map<qg_string, qg_string> header_t;
  qg_string name; // for debugging purposes
  qg_string raw;
  enum http_parser_type type;
  enum http_method method;
  int status_code; // use for response
  qg_string request_path;
  qg_string request_url;
  qg_string fragment;
  qg_string query_string;
  qg_string body;
  enum { NONE = 0, FIELD, VALUE } last_header_element;
  header_t headers;
  std::pair<qg_string, qg_string> last_filed_value;
  int should_keep_alive;

  int body_is_final;
  int message_is_end;
  void printRequest() {
    std::cout << "url : \n"
              << request_url << "\n"
              << "path : \n"
              << request_path << "\n"
              << "query : \n"
              << query_string << "\n"
              << "fragment : \n"
              << fragment << "\n"
              << "header : \n";
    for (const auto &header : headers) {
      std::cout << "[ " << header.first << ", " << header.second << " ]\n";
    }
    std::cout << "body : \n" << body << "\n";
  }
};

} // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_HTTP_REQUEST_H_

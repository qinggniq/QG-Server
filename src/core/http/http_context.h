//
// Created by 王聪 on 2020/3/29.
//

#ifndef QGSERVER_SRC_CORE_HTTP_HTTP_CONTEXT_H_
#define QGSERVER_SRC_CORE_HTTP_HTTP_CONTEXT_H_
#include "../../lib/http-parser/http_parser.h"
class HTTPContext {
public:

private:
  struct http_parser* parser_;
  struct http_parser_settings* settings_;

};

#endif // QGSERVER_SRC_CORE_HTTP_HTTP_CONTEXT_H_

//
// Created by wc on 1/15/19.
//
#pragma once
#ifndef QG_SERVER_REQUESTPARSER_H
#define QG_SERVER_REQUESTPARSER_H

#include "../util/type.h"
#include "http_data.h"

class HttpData;

namespace qg {

class RequestParser {
 public:
  typedef RequestData::method_t method_t;
  typedef RequestData::version_t version_t;
  typedef RequestData::uri_t uri_t;
  typedef RequestData::header_t header_t;
  typedef RequestData::body_t body_t;
  typedef RequestData http_dt;
  typedef qg_int ok_t;
  typedef qg_string msg_t;
 public:
  RequestParser();
  ~RequestParser();
  ok_t Parse(const msg_t &http_msg);
  ok_t Parse(qg_istream &stream);
  http_dt request_data() const;
  void Print() const;

 private:
  ok_t ParseStartLine(const msg_t &msg);
  ok_t ParseMethod(const msg_t &msg);
  ok_t ParseUri(const msg_t &msg);
  ok_t ParseQuery(const msg_t &msg);
  ok_t ParseVersion(const msg_t &msg);
  ok_t ParseHeader(const msg_t &msg);
  ok_t ParseHeader(qg_istream &istream);

  ok_t ParseBody(const msg_t &msg);
  ok_t ParseBody(qg_istream &istream);

  http_dt request_d_;

};
} //namespace qg
#endif //QG_SERVER_REQUESTPARSER_H

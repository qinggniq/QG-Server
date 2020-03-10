//
// Created by wc on 1/15/19.
//
#pragma once
#ifndef QG_SERVER_REQUESTPARSER_H
#define QG_SERVER_REQUESTPARSER_H

#include "type.h"
#include "http_data.h"


namespace qg {

enum class ErrorCode{
  kFileNotFound,
  kStartLineError,
  kQueryError,
  kProtoclError,
  kHeaderError,
  kBodyError,
  kEncodeError
};


class RequestParser {
 public:
  typedef RequestData::method_t method_t;
  typedef RequestData::version_t version_t;
  typedef RequestData::uri_t uri_t;
  typedef RequestData::header_t header_t;
  typedef RequestData::body_t body_t;
  typedef std::shared_ptr<RequestData> http_d_pt;
  typedef qg_string response_t;
  typedef qg_int ok_t;
  typedef qg_string msg_t;
 public:
  explicit RequestParser(http_d_pt http_data) : request_d_(std::move(http_data)) {}
  ~RequestParser();
  ok_t Parse(const msg_t &http_msg);
  ok_t Parse(qg_istream &stream);
  ErrorCode error_code() const {return error_code_;}
  //http_d_pt request_data() const;
  void Print() const;

  //qg_string response() const {return response_;}

 private:

  ok_t ParseQuery(const msg_t &msg);

  ok_t ParseHeader(qg_istream &istream);

  ok_t ParseBody(const msg_t &msg);
  ok_t ParseBody(qg_istream &istream);

  ErrorCode error_code_;
  http_d_pt request_d_;
  //response_t response_;
};
} //namespace qg
#endif //QG_SERVER_REQUESTPARSER_H

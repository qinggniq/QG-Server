//
// author:qinggniq
// email:livewithblank@gmail.com
// Created by wc on 1/15/19.
//
#pragma once
#ifndef QG_SERVER_HTTP_DATA_H
#define QG_SERVER_HTTP_DATA_H

#include <unordered_map>
#include <memory>

#include "../util/type.h"

// ----------------------------------------------------------------------
// Here we define the RequestData Class to represent the Http Header & Body.
// -----------------------------------------------------------------------


// Http has many text meaning definition.
// such that ctl := <the character which >128 >
// Http Http Content (from rfc2616):
// Http = Http-Line
//           *(( general header | Http-header | entity-header ) CRLF)*
//           CRLF
//           [ message-body ]

// Http-Line = Method SP Http-URI SP HTTP-Version CRLF
// Method = "OPTIONS"|"GET"|"POST"|"PUT"... (QG-Server only supports the "GET", "POST", "HEAD" methods.)
//
namespace qg {

const qg_string GET = "GET";
const qg_string HEAD = "HEAD";
const qg_string POST = "POST";

class MIMEType {
 public:
  typedef qg_string mime_vt;
  typedef qg_string mime_kt;
  typedef std::unordered_map<mime_kt, mime_vt> mime_type_map;
 public:
  MIMEType();
  MIMEType(const MIMEType &);
  ~MIMEType();
  static void init();
  static mime_vt mime_item(mime_kt &&suffix);
 public:
  static mime_type_map mime_type_;
};

class RequestData {
 public:
  typedef qg_string method_t;
  typedef qg_string version_t;
  typedef qg_string uri_t;
  typedef qg_string body_t;
  typedef std::unordered_map<qg_string, qg_string> header_t;
  typedef std::unordered_map<qg_string, qg_string> query_t;
  typedef qg_string query_vt;
  typedef qg_string query_kt;
  typedef qg_string header_vt;
  typedef qg_string header_kt;

 public:
  RequestData(
      method_t &&method,
      version_t &&version,
      uri_t &&uri,
      query_t &&query,
      header_t &&header,
      body_t &&body) :
      method_(std::move(method)),
      version_(std::move(version)),
      uri_(std::move(uri)),
      query_(std::move(query)),
      header_(std::move(header)),
      body_(std::move(body)) {}
  RequestData();
  ~RequestData();

  version_t version() const;
  void set_version(const version_t &version);
  method_t method() const;
  void set_methd(const method_t &method);
  uri_t uri() const;
  void set_uri(const uri_t &uri);
  void set_query(const query_t &query);
  void set_file(const qg_string &file){file_ = file;}
  void set_path(const qg_string &path){path_ = path;}
  header_t header() const;
  query_vt query_item(const query_vt &) const;
  qg_string file() const {return file_;}
  qg_string path() const {return path_;}
  //void set_query_item(const query_kt &key, const query_vt &value);
  //void set_header(const header_t &header);
  void set_header_item(const header_kt &key, const header_vt &value);
  header_vt header_item(const header_kt &) const;
  //void set_body(const body_t &body);
  body_t body() const;

 public:
  qg_bool Encoded() const ;
  qg_bool KeepAlive() const;

 private:
  method_t method_;
  version_t version_;
  uri_t uri_;
  qg_string path_;
  qg_string file_;
  query_t query_;
  header_t header_;
  body_t body_;
};

enum StatusCode {
  CONTINUE = 100,
  OK = 200,
  MULTIPLE_CHOICE = 300,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  INTERNAL_SERVER_ERROR = 500,
  BAD_GATEWAY = 501,
  SERVICE_UNAVAILABLE = 503,
  HTTPVERSION_NOT_SUPPORTED = 505
};

/*
 * Response = Status-Line
 *						*(( general-header | response-header | entity-header ) CRLF)
 * 						CRLF
 * 						[ message-body ]
 *
 * Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
*/
class ResponseData {
 public:
  typedef qg_string version_t;
  typedef qg_string body_t;
  typedef std::unordered_map<qg_string, qg_string> header_t;
  typedef qg_string header_vt;
  typedef qg_string header_kt;
  typedef StatusCode status_t;
  typedef qg_string reason_t;

 public:
  ResponseData() = default;
  ~ResponseData() = default;
  version_t version() const {
    return this->version_;
  }
  status_t status() const {
    return this->status_;
  }
  reason_t reason() const {
    return this->reason_;
  }
  header_vt header_item(header_kt &) const;

  header_t header() const {
    return this->header_;
  }

  body_t body() const {
    return this->body_;
  }

  void set_version(const version_t &version) {
    this->version_ = version;
  }

  void set_status(const status_t status) {
    this->status_ = status;
    //TODO (qinggniq) implement the <StatusCode, reason pharse> map
    this->reason_ = "TODO";
  }

  void set_header(const header_t &header) {
    this->header_.insert(header.begin(), header.end());
  }

  void set_header_item(const header_kt &key, const header_vt &val) {
    this->header_.emplace(key, val);
  }

  void set_body(const body_t &body) {
    this->body_ = body;
  }

 private:
  version_t version_;
  status_t status_;
  reason_t reason_;
  header_t header_;
  body_t body_;
};
} //namespace qg

#endif //QG_SERVER_HTTPDATA_H

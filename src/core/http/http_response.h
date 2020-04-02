//
// Created by 王聪 on 2020/3/29.
//

#ifndef QGSERVER_SRC_CORE_HTTP_HTTP_RESPONSE_H_
#define QGSERVER_SRC_CORE_HTTP_HTTP_RESPONSE_H_

#include "../../util/type.h"
namespace qg {
class HTTPResponse {
public:
  HTTPResponse() = default;
  qg_string toString();
  void setStatus(int code) {
    if (code == 200) {
      status_ = "200 OK";
    } else if (code == 404) {
      status_ = "404 Not Found";
    } else if (code == 400) {
      status_ = "400 Bad Request";
    } else if (code == 413) {
      status_ = "413 Payload Too Large";
    } else if (code == 505) {
      status_ = "505 Version Not Supported";
    } else {
      status_ = "500 Internal Server Error";
    }
  }
  qg_string getFileName() const { return file_name_; }
  void setFileName(const qg_string &file_name) { file_name_ = file_name; }
  void setHeader(const qg_string &field, const qg_string &value) {
    headers_.append(field + ": ");
    headers_.append(value + "\r\n");
  }
  void setContent(qg_string &&content) { content_ = std::move(content); }

private:
  qg_string status_;
  qg_string headers_;
  qg_string file_name_;
  qg_string content_;
};

}; // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_HTTP_RESPONSE_H_

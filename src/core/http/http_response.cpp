//
// Created by 王聪 on 2020/3/29.
//

#include "http_response.h"
#include "../../util/time_stamp.h"
#include <sstream>
namespace qg {
qg_string HTTPResponse::toString() {
  qg_ostringstream ss;
  ssize_t  content_len_;
  if (content_.empty()) {
    content_len_ = status_.size();
  } else {
    content_len_ = content_.size();
  }
  ss << "HTTP/1.1 " << status_ << "\r\n";
  ss << "Server: QG Server (MacOs)\r\n";
  ss << "Date: " << TimeStamp::getTimeOfDay() << "\r\n";
  ss << "Content-Type: text/html\r\n";
  ss << "Connection: keep-alive\r\n";
  ss << "Content-Length: " << content_len_ << "\r\n";
  ss << headers_;
  // 这里可能需要根据服务器设定的支不支持keep alive来搞。
  ss << "\r\n";
  if (content_.empty())
    ss << status_;
  else
    ss << content_;
  return std::move(ss.str());
}
} // namespace qg
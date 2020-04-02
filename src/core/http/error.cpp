//
// Created by 王聪 on 2020/4/2.
//

#include "error.h"
#include "../tcp_connection.h"
#include "../../util/time_stamp.h"
#include "../../lib/http-parser/http_parser.h"
#include <sstream>
namespace qg {
void sendError(std::shared_ptr<TcpConnection> conn, int error_code) {
  std::ostringstream ss;
  qg_string status;
  if (error_code == 404) {
    status = "404 Not Found";
  } else if (error_code == 400) {
    status = "400 Bad Request";
  } else if (error_code == 413) {
    status = "413 Payload Too Large";
  } else if (error_code == 505) {
    status = "505 Version Not Supported";
  } else {
    status = "500 Internal Server Error";
  }
  ss << "HTTP/1.1 " << status << "\r\n";
  ss << "Server: Httpd (Ubuntu)\r\n";
  ss << "Date: " << TimeStamp::getTimeOfDay() << "\r\n";
  ss << "Content-Type: text/html\r\n";
  ss << "Content-Length: " << status.size() << "\r\n";
  // 这里可能需要根据服务器设定的支不支持keep alive来搞。
  ss << "Connection: keep-alive\r\n";
  ss << "\r\n";
  ss << status;
  conn->write(ss.str());
}
}; // namespace qg
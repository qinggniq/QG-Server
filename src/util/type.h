//
// Created by wc on 1/15/19.
//

#ifndef QG_SERVER_TYPE_H
#define QG_SERVER_TYPE_H

#include <memory>
#include <sstream>
#include <string>
#include <sys/socket.h>
namespace qg {
using qg_short = short;
using qg_uint = unsigned int;
using qg_size_t = size_t;
using qg_ssize_t = ssize_t;
using qg_time_t = time_t;
using qg_int = int;
using qg_int64_t = int64_t;
using qg_uint64_t = uint64_t;

using qg_long = long;
using qg_longlong = long long;
using qg_istream = std::istream;
using qg_ostringstream = std::ostringstream;
using qg_istringstream = std::istringstream;
using qg_string = std::string;
using qg_double = double;
using qg_bool = bool;
using qg_fd_t = qg_int;
using qg_event_t = qg_int;
using qg_char_t = char;

const qg_int kError = -1;
const qg_int kOk = 0;
const qg_int kTrue = 1;
const qg_int kFalse = 0;
class TcpConnection;
typedef std::string *buf_pt;
typedef std::function<void(std::shared_ptr<TcpConnection>, buf_pt)>
    MessageCallBack;
typedef std::function<void(std::shared_ptr<TcpConnection>, buf_pt)>
    WriteCompleteCallBack;
typedef std::function<void()> ConnectionComeCallBack;
// FIXME(qinggniq): close的时候需不需要buffer是一个问题
typedef std::function<void(std::shared_ptr<TcpConnection>)>
    ConnectionCloseCallBack;
typedef std::function<void()> HandleWriteCallBack;
typedef std::function<void()> timer_handler_cb_t;

//占位符
class Todo {};

// network

} // namespace qg
#endif // QG_SERVER_TYPE_H

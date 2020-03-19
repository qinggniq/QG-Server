//
// Created by wc on 3/14/19.
//

#ifndef SRC_CORE_SOCKET_H_
#define SRC_CORE_SOCKET_H_

#include <string>
#include "../util/noncopyable.h"
#include "../util/type.h"

/*
 *  当使用非阻塞的Socket时，如果需要处理已经断开连接的客户端连接，请设置心跳，或者定时任务。
 *  https://stackoverflow.com/questions/5906175/what-do-a-benefit-from-changing-from-blocking-to-non-blocking-sockets
 */
namespace qg {

class Socket {
public:
  typedef Socket *socket_pt;

  Socket();

  ~Socket();

  explicit Socket(qg_int back_log);

  qg_fd_t sfd() const { return fd_; }

  void makeAddrReuseable();

  void makePortReuseable();

  void makeNonBlock();

  void makeCloseExe();

  void makeCork();

  void makeKeepAlive();

  void bindAndListen(qg_int);

  qg_fd_t accept();

  void close();

  std::string Info() { return info_; }

private:
  void appendInfo(std::string str) { info_ += str; }

  std::string info_;
  qg_fd_t fd_;
  const qg_int back_log_;
};
} // namespace qg

#endif // SRC_CORE_SOCKET_H_

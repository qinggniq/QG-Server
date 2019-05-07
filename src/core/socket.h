//
// Created by wc on 3/14/19.
//

#ifndef SRC_SOCKET_H
#define SRC_SOCKET_H

#include "noncopyable.h"
#include "type.h"


/*
 *  当使用非阻塞的Socket时，如果需要处理已经断开连接的客户端连接，请设置心跳，或者定时任务。
 *  https://stackoverflow.com/questions/5906175/what-do-a-benefit-from-changing-from-blocking-to-non-blocking-sockets
 */
namespace qg{
class Socket {
 public :
  Socket() ;
  explicit Socket(qg_int back_log);
  qg_fd_t sfd() const {return sfd_;};
  void makeAddrReuseable();
  void makePortReuseable();
  void makeNonBlock();
  void makeCloseExe();
  void makeCork();
  void makeKeepAlive();

  void setBackLog(qg_int back_log) {back_log_ = back_log;}
  void bindAndListen(qg_int port);
  qg_fd_t accept();
  void close();

 private:
  qg_fd_t sfd_;
  qg_int back_log_;
};
}
#endif //SRC_SOCKET_H

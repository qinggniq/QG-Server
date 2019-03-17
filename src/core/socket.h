//
// Created by wc on 3/14/19.
//

#ifndef SRC_SOCKET_H
#define SRC_SOCKET_H
#include "noncopyable.h"
#include "type.h"
/*
 * 当使用非阻塞的Socket时，如果需要处理已经断开连接的客户端连接，请设置心跳，或者定时任务。
 *  https://stackoverflow.com/questions/5906175/what-do-a-benefit-from-changing-from-blocking-to-non-blocking-sockets
 */
namespace qg{
class Socket:public noncopyable{
  Socket(qg_int fd,
         qg_bool reuse,
         qg_bool nonblock,
         qg_bool cork):
         sfd_(fd),
         reuse_(reuse),
         nonblock_(nonblock),
         cork_(cork){}

  void makeAddrReuseable();
  void makeNonblock();

 private:
  qg_int sfd_;
  qg_bool reuse_;
  qg_bool nonblock_;
  qg_bool cork_;

};
}
#endif //SRC_SOCKET_H

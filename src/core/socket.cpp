//
// Created by wc on 3/14/19.
//

#include "socket.h"
#include <sys/socket.h>
#include <fcntl.h>

namespace qg{
void Socket::makeAddrReuseable() {
  qg_int enable = 1;
  if (setsockopt(sfd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(qg_int)) < 0);
    //TODO (qinggniq) log
}
/*
 * copyright:
 * author: Jeremy Friesner
 * https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
 */
void Socket::makeNonblock() {
  qg_int flags = fcntl(sfd_, F_GETFL, 0);
  if (flags == -1) ;//TODO (qinggniq) log.
  flags = flags | O_NONBLOCK;
  if (fcntl(sfd_, F_SETFL, flags) != 0); //TODO (qinggniq) log
}

}//namespace qg
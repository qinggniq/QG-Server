//
// Created by wc on 2/15/19.
//

#include "accept_handler_inherit.h"
#include <sys/socket.h>

namespace qg{
  qg_int
  AcceptHandler::HandleRead() {
    qg_int fd;
    struct sockaddr_storage sa;
    socklen_t salen = sizeof(sa);
    while(true) {
      fd = accept(this->handle_, (struct sockaddr*)&sa, salen);
      if (fd == -1) {
        if (errno == EINTR)
          continue;
        else {
          //anetSetError(err, "accept: %s", strerror(errno));
          return kError;
        }
      } else {
        if (sa.ss_family == AF_INET) {
          struct sockaddr_in *s = (struct sockaddr_in *)&sa;
          if (ip) inet_ntop(AF_INET,(void*)&(s->sin_addr),ip,ip_len);
          if (port) *port = ntohs(s->sin_port);
        } else {
          struct sockaddr_in6 *s = (struct sockaddr_in6 *)&sa;
          if (ip) inet_ntop(AF_INET6,(void*)&(s->sin6_addr),ip,ip_len);
          if (port) *port = ntohs(s->sin6_port);
        }
      }
      break;
    }

  }

  void
  AcceptHandler::HandleWrite() {

  }

  void
  AcceptHandler::HandleError() {

  }


}//namespace qg
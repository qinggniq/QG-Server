//
// Created by wc on 2/21/19.
//

#ifndef SRC_NETTOOL_H
#define SRC_NETTOOL_H

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <zconf.h>
#include <glog/logging.h>
#include "type.h"

/*
 * At the book 《The Linux Programing Interface》, it introduce the TCP_CROK
 * options at socket, it's very suitable to HTTP server, so I add it in my
 * QG-server.
 */
#ifdef TCP_CORK
#define QGIO_NOPUSH TCP_CORK
#elif defined(TCP_NOPUSH)
#define QGIO_NOPUSH TCP_NOPUSH
#endif

namespace qg {


static qg_fd_t QGAccept(qg_fd_t sfd, struct sockaddr *sockaddr, socklen_t *slen) {
  qg_fd_t cfd;
  if ((cfd = accept(sfd, sockaddr, slen)) != -1) {
    // Maybe we need save the information of the new client.
    return cfd;
  } else {
    // log
    return -1;
  }
}

/*
 * The implmenation of the wrap function is copy from 《The Linux Programing
 * Interface》
 */

/* Public interfaces: inetBind() and inetListen() */
static qg_fd_t QGInetPassiveSocket(const qg_char_t *service, qg_int type,
                                   socklen_t *addrlen, qg_bool doListen,
                                   qg_int backlog) {
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, optval, s;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;
  hints.ai_socktype = type;
  hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
  hints.ai_flags = AI_PASSIVE; /* Use wildcard IP address */
  s = getaddrinfo(nullptr, service, &hints, &result);

  if (s != 0)
    return -1;

  /* Walk through returned list until we find an address structure
        that can be used to successfully create and bind a socket */
  optval = 1;
  for (rp = result; rp != nullptr; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue; /* On error, try next address */
    if (doListen) {
      if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
          -1) {
        close(sfd);
        freeaddrinfo(result);
        return -1;
      }
    }
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */

    /* bind() failed: close this socket and try next address */
    close(sfd);
  }
  if (rp != nullptr && doListen) {
    if (listen(sfd, backlog) == -1) {
      freeaddrinfo(result);
      return -1;
    }
  }
  if (rp != nullptr && addrlen != nullptr)
    *addrlen = rp->ai_addrlen;
  freeaddrinfo(result);
  return (rp == nullptr) ? -1 : sfd;
}

static qg_fd_t QGNetListen(qg_size_t port, socklen_t *addlen, qg_int backlog) {
  port = ((port < 1024 || port > 65535)) ? 6666 : port;
  qg_string sport = std::to_string(port);
  return QGInetPassiveSocket(sport.c_str(), SOCK_STREAM, addlen, true, backlog);
}

static qg_int QGSetTcpCork(qg_fd_t sfd) {
  qg_int optval = 1;
  if (setsockopt(sfd, SOL_SOCKET, QGIO_NOPUSH, &optval, sizeof(optval)) == -1) {
    close(sfd);
    return -1;
  }
  return 0;
}

static void makeFdNoneBlock(qg_fd_t fd) {
  qg_int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    LOG(ERROR) << "error when get flags";
  }
  flags = flags | O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) != 0) {
    LOG(ERROR) << "error when set O_NONBLOCK flags";
  }
}

} // namespace qg
#endif // SRC_NETTOOL_H

//
// Created by wc on 3/14/19.
//
#include "socket.h"

#include <fcntl.h>
#include <glog/logging.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef TCP_CORK
#define QGIO_NOPUSH TCP_CORK
#elif defined(TCP_NOPUSH)
#define QGIO_NOPUSH TCP_NOPUSH
#endif
#define BACKLOG 5
namespace qg {
Socket::Socket() {
  if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    // TODO log error
    switch (errno) {
    default:
      break;
    };
  }
  info_ += "fd : " + std::to_string(fd_) + "\n";
  info_ += "IPV4\n";
}

Socket::Socket(qg_fd_t fd) : fd_(fd) {
  info_ += "fd : " + std::to_string(fd_) + "\n";
}

Socket::~Socket() { ::close(fd_); }

void Socket::makeAddrReuseable() {
  qg_int enable = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(qg_int)) < 0) {
    LOG(WARNING) << "fail to make fd " << fd_ << " port reuseable";
    return;
  }
  LOG(INFO) << "success to make fd " << fd_ << " port reuseable";
}

void Socket::makePortReuseable() {
  qg_int enable = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &enable,
                 static_cast<socklen_t>(sizeof(qg_int))) < 0) {
    LOG(WARNING) << "fail to make fd " << fd_ << " port reuseable";
    return;
  }
  LOG(INFO) << "success to make fd " << fd_ << " port reuseable";
}

void Socket::makeKeepAlive() {
  qg_int enable = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &enable,
                 static_cast<socklen_t>(sizeof(qg_int))) < 0) {
    LOG(WARNING) << "fail to make fd " << fd_ << " keep alive";
    return;
  }
  LOG(INFO) << "success to make fd " << fd_ << " keep alive";
}

void Socket::makeCork() {
  qg_int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, QGIO_NOPUSH, &optval, sizeof(optval)) == -1) {
    LOG(WARNING) << "fail to make fd " << fd_ << " cork";
    return;
  }
  LOG(INFO) << "success to make fd " << fd_ << " cork";
}

void Socket::makeNonBlock() {
  qg_int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    LOG(ERROR) << "error when get flags";
  }
  flags = flags | O_NONBLOCK;
  if (fcntl(fd_, F_SETFL, flags) != 0) {
    LOG(ERROR) << "error when set O_NONBLOCK flags";
  }
}

void Socket::makeCloseExe() {
  qg_int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    LOG(ERROR) << "error when get flags";
  }
  flags = flags | O_CLOEXEC;
  if (fcntl(fd_, F_SETFL, flags) != 0) {
    LOG(ERROR) << "error when set CloseExe flags";
  }
}

void Socket::close() {
  qg_int ret = ::close(fd_);
  if (ret != 0) {
    if (errno == EBADF) {
      // TODO log isn't valid
    }
    if (errno == EINTR) {
      // TODO log interrupted by signal
    }
    if (errno == EIO) {
      // TODO log error happen in IO
    }
  }
  info_ += "closed\n";
}

void Socket::bindAndListen(qg_int port, qg_int backlog) {
  LOG(INFO) << "bind and listen on port " << port;
  if (port < 0) {
    port = 3000;
  }
  struct sockaddr_in serv_addr;
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons((unsigned short)port);
  qg_int ret = ::bind(fd_, (struct sockaddr *)&serv_addr,
                      static_cast<socklen_t>(sizeof(struct sockaddr)));
  assert(ret == 0);
  if (ret != 0) {
    // LOG
    switch (errno) {
    case EADDRINUSE:
      // TODO log address in use
      break;
    case EBADF:
      // TODO log not valid fd
      break;
    case EINVAL:
      // TODO log socket in use
      break;
    case ENOTSOCK:
      // TODO sockfd is not socket fd
      break;
    case EACCES:
      // TODO log error permission
      break;
    case EADDRNOTAVAIL:
      // TODO log requested address was not local
      break;
    case EFAULT:
      // TODO log addr points outside of use's accessible address space
      break;
    case ELOOP:
      // TODO log too many link
      break;
    case ENOENT:
      // TODO file not exist
      break;
    case ENAMETOOLONG:
      // TODO name too long
      break;
    case ENOMEM:
      // TODO insufficient kernel memory
      break;
    case ENOTDIR:
      // TODO not a directory
      break;
    case EROFS:
      // TODO read-only file system
      break;
    default:
      break;
    };
  }
  LOG(INFO) << "listen on fd : " << fd_;
  if (listen(fd_, backlog) == -1) {
    // TODO log error
    switch (errno) {
    case EBADF:
      break;
    case EDESTADDRREQ:
      break;
    case EINVAL:
      break;
    case ENOTSOCK:
      break;
    case EOPNOTSUPP:
      break;
    default:
      break;
    };
  }
  info_ += "Listen on port: " + std::to_string(port);
  LOG(INFO) << "bind end";
}

// TODO: 客户端的信息应该在accpet的时候填入。。
qg_fd_t Socket::accept() {
  struct sockaddr cli_addr;
  socklen_t addr_len = sizeof(cli_addr);
  qg_fd_t new_fd = ::accept(fd_, &cli_addr, &addr_len);
  // TODO log the new Connections
  /*
   * exmple:
   * LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" <<
   * ntohs(client_addr.sin_port);
   */
  if (new_fd < 0) {
    // TODO log accept error
    switch (errno) {
    case EAGAIN:
      // TODO socket noblock and no connetion on queue
      break;
    case EBADF:
      break;
    case ECONNABORTED:
      // TODO connection aborted
      break;
    case EFAULT:
      // TODO addr not writeable
      break;
    case EINTR:
      break;
    case EINVAL:
      // TODO socket is not listening
      break;
    case EMFILE:
      // TODO open file descriptor num limit
      break;
    case ENOBUFS:
      // TODO no memory;
      break;
    case ENOTSOCK:
      // TODO not socket
      break;
    case EOPNOTSUPP:
      // TODO not SOCK_STREAM
      break;
    case EPROTO:
      // TODO protocol error
      break;
    case EPERM:
      // TODO firewall rules forbid
      break;
    default:
      break;
    }
    return -1;
  }
  return new_fd;
}

} // namespace qg
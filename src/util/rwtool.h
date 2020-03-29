//
// Created by wc on 2/21/19.
//

#ifndef SRC_READ_WRITE_TOOL_H
#define SRC_READ_WRITE_TOOL_H
#include "type.h"
#include <cstdlib>
#include <glog/logging.h>
#include <zconf.h>

namespace qg {
// The max size of one read() read.
const qg_size_t kBufferSize = 4096;

/*
 * The QGReadN, QGWriteN performs the NONBLOCKING read/write, for in nonblocking
 * IO, read and write system callback may not read write the needed number of
 * bytes.
 */
qg_ssize_t QGReadN(qg_fd_t sfd, qg_char_t *buffer, qg_size_t size) {
  qg_ssize_t nread;
  qg_size_t nleft = size;
  qg_char_t *bptr = buffer;
  qg_size_t nsum = 0;
  while (nleft > 0) {
    nread = read(sfd, bptr, nleft);
    if (nread > 0) {
      nsum += nread;
      bptr += nread;
      nleft -= nread;
    } else if (nread == 0) {
      break;
    } else {
      // TODO(qinggniq) Error Handle
      if (errno == EAGAIN) {
        return nsum;
      }
      return -1;
    }
  }
  return nsum;
}

qg_size_t QGReadN(qg_fd_t sfd, qg_string &buffer, qg_size_t size) {
  // TODO (qinggniq) optimize the implemenation.
  auto *cbuffer = new qg_char_t[size]();
  qg_size_t nread;
  if ((nread = QGReadN(sfd, cbuffer, size)) > 0) {
    buffer += qg_string(cbuffer);
    switch (errno) {
    case EAGAIN:
      LOG(INFO) << "::read() again";
      break;
    case ECONNRESET:
      LOG(INFO) << "::read() peer closed";
      break;
    default:
      LOG(INFO) << "::read() error";
    }
  };
  delete[] cbuffer;
  return nread;
}

qg_ssize_t QGWriteN(qg_fd_t sfd, const char *buffer, qg_size_t size) {
  qg_ssize_t nwrite;
  if ((nwrite = write(sfd, buffer, size)) < 0) {
    nwrite = -1;
    switch (errno) {
    case EAGAIN:
      LOG(INFO) << "::write() again";
      break;
    case ECONNRESET:
      LOG(INFO) << "::write() peer closed";
      break;
    default:
      LOG(INFO) << "::write() error";
    }
  }
  return nwrite;
}

qg_size_t QGWriteN(qg_fd_t sfd, qg_string &buffer, qg_size_t size) {
  return QGWriteN(sfd, buffer.c_str(), size);
}

} // namespace qg
#endif // SRC_READ_WRITE_TOOL_H

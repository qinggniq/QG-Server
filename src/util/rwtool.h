//
// Created by wc on 2/21/19.
//

#ifndef SRC_READ_WRITE_TOOL_H
#define SRC_READ_WRITE_TOOL_H
#include "type.h"
#include <cstdlib>
#include <fstream>
#include <glog/logging.h>
#include <utility>
#include <zconf.h>

namespace qg {
// The max size of one read() read.
const qg_size_t kBufferSize = 4096;

/*
 * The QGReadN, QGWriteN performs the NONBLOCKING read/write, for in nonblocking
 * IO, read and write system callback may not read write the needed number of
 * bytes.
 */
static qg_ssize_t read(qg_fd_t sfd, qg_char_t *buffer, qg_size_t size) {
  qg_ssize_t nread;
  qg_size_t nleft = size;
  qg_char_t *bptr = buffer;
  qg_size_t nsum = 0;
  while (nleft > 0) {
    nread = ::read(sfd, bptr, nleft);
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

static qg_size_t read(qg_fd_t sfd, qg_string &buffer, qg_size_t size) {
  // TODO (qinggniq) optimize the implemenation.
  auto cbuffer = new qg_char_t[size]();
  qg_ssize_t nread, cnt = 0;
  while (true) {
    nread = ::read(sfd, cbuffer, size);
    if (nread > 0) {
      cnt += nread;
      LOG(INFO) << "read n " << nread << " bytes";
      buffer += qg_string(cbuffer, nread);
      LOG(INFO) << "read end";
      if (nread != size) {
        break;
      }
    } else if (nread == 0) {
      return cnt;
    } else {
      switch (errno) {
      case EAGAIN:
        LOG(INFO) << "::read() again";
        goto normal_end;
      case ECONNRESET:
        LOG(INFO) << "::read() peer closed";
        goto error;
      default:
        LOG(INFO) << "::read() encounte error";
        goto error;
      }
    }
  }
normal_end:
  delete[] cbuffer;
  return nread;
error:
  delete[] cbuffer;
  return -1;
}

static qg_ssize_t write(qg_fd_t sfd, const char *buffer, qg_size_t size) {
  ssize_t nwrite, cnt = 0;
  size_t total = size;
  const char *p = buffer;

  while (1) {
    nwrite = ::write(sfd, p, total);
    if (nwrite < 0) {
      // 当send收到信号时,可以继续写,但这里返回-1.
      if (errno == EINTR)
        return -1;
      // 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
      // 在这里做延时后再重试.
      if (errno == EAGAIN) {
        break;
      }
      return -1;
    }
    cnt += nwrite;
    if ((size_t)cnt == total)
      return size;
  }
  return cnt;
}

static qg_size_t write(qg_fd_t sfd, const qg_string &buffer, qg_size_t size) {
  return write(sfd, buffer.c_str(), size);
}

static qg_string readFile(const qg_string &file_name) {
  std::ifstream ifs(file_name.c_str(),
                    std::ios::in | std::ios::binary | std::ios::ate);
  std::ifstream::pos_type file_size = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  std::vector<char> bytes(file_size);
  ifs.read(bytes.data(), file_size);
  return std::move(qg_string(bytes.data(), file_size));
}

} // namespace qg
#endif // SRC_READ_WRITE_TOOL_H

//
// Created by wc on 2/21/19.
//

#ifndef SRC_READ_WRITE_TOOL_H
#define SRC_READ_WRITE_TOOL_H
#include "type.h"
#include <cstdlib>
#include <zconf.h>

namespace qg{
//The max size of one read() read.
const qg_size_t kBufferSize = 4096;

/*
 * The QGReadN, QGWriteN performs the NONBLOCKING read/write, for in nonblocking IO,
 * read and write system callbcak may not read write the needed number of bytes.
 */
qg_size_t QGReadN(qg_fd_t sfd, qg_char_t* buffer, qg_size_t size) {
  qg_ssize_t nread;
  qg_size_t nleft = size;
  qg_char_t* bptr = buffer;
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
      //TODO (qinggniq) Error Handle
      //Here exist the EAGAIN ERROR
      if (errno == EAGAIN) {
        return nsum;
      }
      return static_cast<qg_size_t >(-1);
    }
  }
  return nsum;
}

qg_size_t QGReadN(qg_fd_t sfd, qg_string& buffer, qg_size_t size) {

  auto *cbuffer = new qg_char_t[size](); // TODO (qinggniq) optimize the implemenation.
  //qg_char_t cbuffer[kBufferSize];
  qg_size_t nread;

  if ((nread = QGReadN(sfd, cbuffer, size)) > 0) {
    buffer = cbuffer;
    //TODO (qinggniq) here maybe produce many fragments of memory, so use stack?
  };
  delete[] cbuffer;

  return nread;
}

qg_size_t QGWriteN(qg_fd_t sfd, const char* buffer, qg_size_t size) {
  qg_ssize_t nwrite;
  if ((nwrite = write(sfd, buffer, size)) < 0) {
    //Log
    nwrite = -1;
  }
  return static_cast<qg_size_t>(nwrite);
}

qg_size_t QGWriteN(qg_fd_t sfd, qg_string& buffer, qg_size_t size) {
  return QGWriteN(sfd, buffer.c_str(), size);
}

} //namespace qg
#endif //SRC_READ_WRITE_TOOL_H

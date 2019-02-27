//
// Created by wc on 2/21/19.
//

#ifndef SRC_REQUEST_HANDLER_INHERIT_H
#define SRC_REQUEST_HANDLER_INHERIT_H
#include "event_handler_inherit.h"

namespace qg{

class EventHandler;

class RequestHandler:public EventHandler{
 public:
  RequestHandler(qg_int fd, events_t ie):EventHandler(fd, ie) {}
  ~RequestHandler() = default;
 protected:
  void HandleRead();
  void HandleWrite();
  void HandleError();
};

}
#endif //SRC_REQUEST_HANDLER_INHERIT_H

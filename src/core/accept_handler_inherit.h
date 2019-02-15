//
// Created by wc on 2/15/19.
//

#ifndef SRC_LOGIN_HANDLER_H
#define SRC_LOGIN_HANDLER_H

#include "type.h"
#include "event_handler_inherit.h"

namespace qg{
  class EventHandler;
  class AcceptHandler:public EventHandler {
   protected:
    void HandleRead();
    void HandleWrite();
    void HandleError();
  };


class SockHandler : public EventHandler {
 public:
  void HandleEvent();
  SockHandler() = default;
  ~SockHandler(){
    //TODO
  }
 private:

};
}//namespace qg

#endif //SRC_LOGIN_HANDLER_H

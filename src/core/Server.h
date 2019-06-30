//
// Created by wc on 5/7/19.
//

#ifndef SRC_ACCPETOR_H
#define SRC_ACCPETOR_H
#include <memory>
#include "noncopyable.h"
#include "type.h"

namespace qg{

class Dispatcher;
class Socket;
class EventHandler;
class Config;

class Server : public qg::noncopyable{
 public:
  typedef std::shared_ptr<Dispatcher> dispatcher_pt;
  typedef std::shared_ptr<Socket> socket_pt;
  typedef std::shared_ptr<EventHandler> event_handler_pt;
  typedef std::shared_ptr<Config> config_pt;

  explicit Server(dispatcher_pt dispatcher,
  	config_pt config);

  void run();
  qg_bool runnig() const {return running_;}
  void handleNewCon();

  //void handleRead();

 private:
  dispatcher_pt dispatcher_;
  config_pt config_;

  qg_bool running_;
  socket_pt socket_;
  event_handler_pt handler_;
};

} //namespace qg


#endif //SRC_ACCPETOR_H

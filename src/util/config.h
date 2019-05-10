//
// Created by wc on 5/7/19.
//

#ifndef SRC_CONFIG_H
#define SRC_CONFIG_H
#include "type.h"
namespace qg{

struct DetailConfig{
  qg_int back_log:4;
  qg_int keep_alive_timeout:4;
  qg_bool cork:1;
  qg_bool sendfile:1;

};
struct Config{
  struct DetailConfig detail;
  qg_int port;
  qg_int time_out;
  qg_int max_connections;
  qg_bool keep_alive;
};

}
#endif //SRC_CONFIG_H

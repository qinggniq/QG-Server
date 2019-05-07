//
// Created by wc on 5/7/19.
//

#ifndef SRC_CONFIG_H
#define SRC_CONFIG_H
#include "type.h"
namespace qg{

struct DetailConfig{
  qg_int back_log;
  qg_bool cork;
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

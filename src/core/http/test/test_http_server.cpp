//
// Created by 王聪 on 2020/4/2.
//
#include <iostream>
#include <glog/logging.h>
#include "../http_server.h"
#include "../../../util/config.h"
using namespace std;
using namespace qg;

int main(int argc, char ** argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = 1;
  Config *config = new Config;
  cout  << " ? " << endl;
  config->keep_alive = true;
  config->max_connections = 100;
  config->port = 1278;
  config->time_out = 0;
  config->detail.back_log = 10;
  config->detail.cork = true;
  config->detail.keep_alive_timeout = 100;
  config->detail.sendfile = true;
  cout  << " ? " << endl;
  HTTPServer* server = new HTTPServer(config);
  server->run();
}

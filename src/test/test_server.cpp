//
// Created by 王聪 on 2020/3/22.
//
#include <glog/logging.h>
#include "../core/server.h"
#include "../core/event_loop.h"
#include "../util/config.h"
#include "../core/tcp_connection.h"
#include <iostream>
using namespace std;
using namespace qg;

int main(int argc, char **argv) {
  FLAGS_log_dir = "/Users/qinggniq/Git/QG-Server/log";
//  google::SetLogDestination(google::GLOG_INFO,"" );
  google::InitGoogleLogging(argv[0]);
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
  Server server(config);

  LOG(INFO) << "before run";
  server.setConnectionComeCallBack([](std::shared_ptr<TcpConnection> _){cout << "connection success" << endl;});
  server.setMessageCallback([](shared_ptr<TcpConnection> conn, buf_t read_buf) {
    cout << "recv : " << (read_buf)  << endl;
    conn->write(read_buf);
  });
  server.setWriteCompleteCallBack([](shared_ptr<TcpConnection> conn, buf_t buf) {
    cout << "write" << endl;
  });
  server.setConnectionCloseCallBack([](shared_ptr<TcpConnection> connd) {
    cout << "closed" << endl;
  });
  server.run();
  LOG(FATAL) << "error exit\n";
}
//
// Created by 王聪 on 2020/4/2.
//

#ifndef QGSERVER_SRC_CORE_HTTP_ROUTER_H_
#define QGSERVER_SRC_CORE_HTTP_ROUTER_H_
#include "../../util/type.h"
#include <map>
namespace qg {
class Router {
public:
  typedef std::map<int, std::map<qg_string, RequestCallBack>> router_t;
  void route(const qg_string &path, int method, RequestCallBack cb) {
    router_[method][path] = cb;
  };
private:
  int type;
  router_t router_;
};
}; // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_ROUTER_H_

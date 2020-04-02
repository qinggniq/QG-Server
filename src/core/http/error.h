//
// Created by 王聪 on 2020/4/2.
//

#ifndef QGSERVER_SRC_CORE_HTTP_ERROR_H_
#define QGSERVER_SRC_CORE_HTTP_ERROR_H_
#include "../tcp_connection.h"
namespace qg {
void sendError(std::shared_ptr<TcpConnection>, int error_code);
}; // namespace qg

#endif // QGSERVER_SRC_CORE_HTTP_ERROR_H_

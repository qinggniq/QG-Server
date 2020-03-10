//
// Created by wc on 3/14/19.
//
#include "socket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netdb.h>


#ifdef TCP_CORK
#	define QGIO_NOPUSH TCP_CORK
#elif defined(TCP_NOPUSH)
#	define QGIO_NOPUSH TCP_NOPUSH
#endif
#define BACKLOG 5
namespace qg {
    Socket::Socket() : Socket(BACKLOG) {}

    Socket::Socket(qg::qg_int back_log) : back_log_(back_log) {
        //目前只有IPv4地址的TCP连接
        if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            //TODO log error
            switch (errno) {
                default:
                    break;
            };
        }
        info_ += "fd : " + std::to_string(fd_) + "\n";
        info_ += "IPV4\n";
    }

     void Socket::makeAddrReuseable( ) {
        qg_int enable = 1;
        if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(qg_int)) < 0);
        //TODO (qinggniq) log
    }

    void Socket::makePortReuseable() {
        qg_int enable = 1;
        if (setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &enable, static_cast<socklen_t >(sizeof(qg_int))) < 0);
        //TODO (qinggniq) log

    }

     void Socket::makeKeepAlive() {
        qg_int enable = 1;
        if (setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &enable, static_cast<socklen_t>(sizeof(qg_int))) < 0);
        //TODO (qinggniq) log
    }

    void Socket::makeCork() {
        qg_int optval = 1;
        if (setsockopt(fd_, SOL_SOCKET, QGIO_NOPUSH, &optval, sizeof(optval)) == -1) {
            //TODO log
            //close(sfd_);
        }
    }

/*
 * copyright:
 * author: Jeremy Friesner
 * https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
 */
    void Socket::makeNonBlock() {
        qg_int flags = fcntl(fd_, F_GETFL, 0);
        if (flags == -1);//TODO (qinggniq) log.
        flags = flags | O_NONBLOCK;
        if (fcntl(fd_, F_SETFL, flags) != 0); //TODO (qinggniq) log
    }

    void Socket::makeCloseExe() {
        qg_int flags = fcntl(fd_, F_GETFL, 0);
        if (flags == -1);//TODO (qinggniq) log.
        flags = flags | O_CLOEXEC;
        if (fcntl(fd_, F_SETFL, flags) != 0); //TODO (qinggniq) log

    }


    void Socket::close() {
        qg_int ret = ::close(fd_);
        if (ret != 0) {
            if (errno == EBADF) {
                //TODO log isn't valid
            }
            if (errno == EINTR) {
                //TODO log interrupted by signal
            }
            if (errno == EIO) {
                //TODO log error happen in IO
            }
        }
        info_ += "closed\n";
    }

    void Socket::bindAndListen(qg_int port) {
        if (port < 0) {
            port = 3000;
        }
        struct sockaddr_in serv_addr;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons((unsigned short) port);
        qg_int ret = ::bind(fd_, (struct sockaddr *) &serv_addr, static_cast<socklen_t>(sizeof(struct sockaddr)));
        if (ret != 0) {
            //LOG
            switch (errno) {
                case EADDRINUSE:
                    //TODO log address in use
                    break;
                case EBADF:
                    //TODO log not valid fd
                    break;
                case EINVAL:
                    //TODO log socket in use
                    break;
                case ENOTSOCK:
                    //TODO sockfd is not socket fd
                    break;
                case EACCES:
                    //TODO log error permission
                    break;
                case EADDRNOTAVAIL:
                    //TODO log requested address was not local
                    break;
                case EFAULT:
                    //TODO log addr points outside of use's accessible address space
                    break;
                case ELOOP:
                    //TODO log too many link
                    break;
                case ENOENT:
                    //TODO file not exist
                    break;
                case ENAMETOOLONG:
                    //TODO name too long
                    break;
                case ENOMEM:
                    //TODO insufficient kernel memory
                    break;
                case ENOTDIR:
                    //TODO not a directory
                    break;
                case EROFS:
                    //TODO read-only file system
                    break;
                default:
                    break;
            };
            if (listen(fd_, back_log_) == -1) {
                //TODO log error
                switch (errno) {
                    case EBADF:
                        break;
                    case EDESTADDRREQ:
                        break;
                    case EINVAL:
                        break;
                    case ENOTSOCK:
                        break;
                    case EOPNOTSUPP:
                        break;
                    default:
                        break;

                };

            }
        }
        info_ += "Listen on port: " + std::to_string(port);
    }

    //TODO: 客户端的信息应该在accpet的时候填入。。
    Socket::socket_pt Socket::accept() {
        struct sockaddr cli_addr;
        socklen_t addr_len = sizeof(cli_addr);
        qg_fd_t new_fd = ::accept(fd_, &cli_addr, &addr_len);
        //TODO log the new Connections
        /*
         * exmple:
         * LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);
         */
        if (new_fd < 0) {
            //TODO log accept error
            switch (errno) {
                case EAGAIN:
                    //TODO socket noblock and no connetion on queue
                    break;
                case EBADF:
                    break;
                case ECONNABORTED:
                    //TODO connection aborted
                    break;
                case EFAULT:
                    //TODO addr not writeable
                    break;
                case EINTR:
                    break;
                case EINVAL:
                    //TODO socket is not listening
                    break;
                case EMFILE:
                    //TODO open file descriptor num limit
                    break;
                case ENOBUFS:
                    //TODO no memory;
                    break;
                case ENOTSOCK:
                    //TODO not socket
                    break;
                case EOPNOTSUPP:
                    //TODO not SOCK_STREAM
                    break;
                case EPROTO:
                    //TODO protocol error
                    break;
                case EPERM:
                    //TODO firewall rules forbid
                    break;
                default:
                    break;
            }
            return nullptr;
        }
        socket_pt new_socket = new Socket(new_fd);
        new_socket->appendInfo("Addr : " + std::string(cli_addr.sa_data));
        new_socket->appendInfo("Remote Port : " ); //TODO;
        return new_socket;
    }

}//namespace qg
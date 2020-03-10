//
// Created by wc on 5/7/19.
//

#ifndef SRC_ACCPETOR_H
#define SRC_ACCPETOR_H

#include <memory>
#include "../util/noncopyable.h"
#include "../util/type.h"

namespace qg {

    class EventLoop;

    class Socket;

    class EventHandler;

    class Config;

    class Accepter : public qg::noncopyable {
    public:
        typedef EventLoop *event_loop_pt;
        typedef EventHandler *event_handler_pt;
        typedef Socket *socket_pt;
        typedef Config *config_pt;

        explicit Accepter(event_loop_pt el,
                        config_pt config);

        void run();

        void handleNewCon();

    private:
        config_pt config_;
        socket_pt socket_;
        event_handler_pt handler_;
        event_loop_pt event_loop_;
    };

} //namespace qg


#endif //SRC_ACCPETOR_H

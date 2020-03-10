//
// Created by 王聪 on 2020/3/7.
//

#ifndef QG_SERVER_POLLER_H
#define QG_SERVER_POLLER_H

#include <vector>
namespace qg {
    class EventHandler;
    class Poller {
    public:
        typedef EventHandler* handler;
        virtual void registerHandler(handler) = 0;
        virtual void removeHandler(handler) = 0;
        virtual void updateHandler(handler) = 0;
        virtual std::vector<handler> Wait(int sz) = 0;
    };
}//namespace qg

#endif //QG_SERVER_POLLER_H

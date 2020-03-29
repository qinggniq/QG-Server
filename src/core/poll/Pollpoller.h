//
// Created by 王聪 on 2020/3/7.
//

#ifndef QG_SERVER_POLLPOLLER_H
#define QG_SERVER_POLLPOLLER_H

#include "Poller.h"
#include "../../util/type.h"
#include "../../util/time_stamp.h"
#include <unordered_map>
#include <sys/poll.h>

namespace qg{
    class Pollpoller : public Poller {
    public:
        typedef std::vector<pollfd> fd_list_t;
        Pollpoller();
        ~Pollpoller();
        void registerHandler(handler) override ;
        void updateHandler(handler) override ;
        void removeHandler(handler) override;
        std::vector<handler> Wait(int sz, TimeStamp time_stamp) override;
    private:
        fd_list_t fds;
    };
}//namespace qg




#endif //QG_SERVER_POLLPOLLER_H

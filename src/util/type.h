//
// Created by wc on 1/15/19.
//

#ifndef QG_SERVER_TYPE_H
#define QG_SERVER_TYPE_H

#include <string>

namespace qg{
#define qg_short short
#define qg_int int
#define qg_long long
#define qg_longlong long long
#define qg_string std::string
    enum Bool{
        TRUE=1,
        FALSE=0,
    };
#define qg_bool Bool
#define TRUE 1
#define FALSE 0
} //namespace qg
#endif //QG_SERVER_TYPE_H

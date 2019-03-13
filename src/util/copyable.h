//
// Created by wc on 3/6/19.
//

#ifndef SRC_COPYABLE_H
#define SRC_COPYABLE_H

/*
 * The copyable class emphsize the fact that the class
 * inherit from it should always be value type.
 */
namespace qg{
class copyable
{
 protected:
  copyable() = default;
  ~copyable() = default;
};
}//namesapce qg
#endif //SRC_COPYABLE_H

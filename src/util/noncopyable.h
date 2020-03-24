//
// Created by wc on 3/13/19.
//

#ifndef SRC_NOCOPYABLE_H
#define SRC_NOCOPYABLE_H
namespace qg {

/*
 * In C++11, this is the best programing practice to implement the non-copyable
 * class. And for declare the use class of copy, so std::move won't generate.
 */
class noncopyable {
public:
  noncopyable(const noncopyable &a) = delete;
  noncopyable &operator=(const noncopyable &a) = delete;

protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
};  // namespace qg
#endif  // SRC_NOCOPYABLE_H

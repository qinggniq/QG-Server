//
// Created by wc on 3/13/19.
//

#ifndef SRC_ATOMIC_H
#define SRC_ATOMIC_H

#include "type.h"
#include "noncopyable.h"
#include <stdint.h>

namespace qg{
/*
 * To implement a thread safe counting has three ways
 * 1. mem lock.
 * 2. gcc __sync_** functions.
 * 3. C++11 Atomic<int> class.
 * The first method is very slow compare to the others, and
 * the differece between 2 and 3 is before gcc4.4, the Atomic<int>::fetch_add()
 * won't be optimized. So although I can use C++11 new feature to implement
 * it, but I still use the __sync__** to deep my knowledge.
 */
class AtomicInt:public noncopyable {
 public:
  AtomicInt():value_(0) {}

  qg_int get() {
    return __sync_val_compare_and_swap(&value_, 0, 0);
  }

  qg_int getAndAdd(qg_int x) {
    return __sync_fetch_and_add(&value_, x);
  }

  qg_int addAndGet(qg_int x) {
    return getAndAdd(x) + x;
  }

  qg_int incrementAndGet() {
    return addAndGet(1);
  }

  qg_int decrementAndGet() {
    return addAndGet(-1);
  }

  void add(qg_int x) {
    getAndAdd(x);
  }

  void increment() {
    incrementAndGet();
  }

  void decrement() {
    decrementAndGet();
  }

  qg_int getAndSet(qg_int x) {
    return __sync_lock_test_and_set(&value_, x);
  }

 private:
  qg_int value_;
};
} // namespace qg
#endif //SRC_ATOMIC_H

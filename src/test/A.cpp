//
// Created by 王聪 on 2020/3/29.
//

#include <iostream>
#include <mutex>
#include <stdlib.h>
#include <thread>
#include <vector>
using namespace std;

// class A {
// public:
//  virtual void F() {
//    cout << "A call F()" << endl;
//  }
//  void callF() {
//    cout << "A call callF()" << endl;
//    F();
//  }
//  virtual ~A() {
//    cout << "A call ~A()" << endl;
//  }
//};
//
// class B : public A{
// public:
//  virtual void F() override {
//    cout << "B call F()" << endl;
//  }
//  void callF() {
//    cout << "B call callF()" << endl;
//    F();
//  }
//  virtual ~B() override  {
//    cout << "B call ~B()" << endl;
//  }
//};

template <typename T>
class Queue  {
public:
  Queue() = default;
  void push(const T& t) {
    std::unique_lock<std::mutex> ulock(mutex_);
    queue_.push_back(t);
    not_empty_.notify_one();
  }
  T pop() {
    std::unique_lock<std::mutex> ulock(mutex_);
    while (queue_.empty()) {
      not_empty_.wait(ulock);
    }
    T res = queue_.back();
    cout << "pop " << res << endl;
    queue_.pop_back();
    return res;
  }
  void pop_all(std::vector<T>& res) {
    std::unique_lock<std::mutex> ulock(mutex_);
    std::swap(res, queue_);
  }
private:
  std::mutex mutex_;
  std::condition_variable not_empty_;
  std::vector<T> queue_;
};
Queue<int> q;

void threadFunc() {
  for (int i = 0; i < 100; ++i) {
    q.push(i);
  }
}

int main() {
  std::thread thread_(threadFunc);
  for (int i = 0; i < 100; ++i) {
    q.pop();
  }
  thread_.join();
}

// int main() {
//  cout << "start " << endl;
//    A* a = new B();
//    a->callF();
//    delete a;
//    return 0;
//}

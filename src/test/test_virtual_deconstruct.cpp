//
// Created by 王聪 on 2020/3/30.
//
#include <iostream>
#define BB
#ifdef BB
#define Obj B
#else
#define Obj C
#endif
using namespace std;

void print(const char* str) {
  cout << str << endl;
}
class A {
public:
  virtual ~A() = 0;
};

class B : public A {
public:
  ~B() override {
      print("~B()");
  }
};

class C : public A {
public:
  ~C() override {
    print("~C()");
  }
};



int main() {
  A* a = new B();
}

//
// Created by 王聪 on 2020/3/29.
//

#include <iostream>
#include <vector>

using namespace std;

class A {
public:
  virtual void F() {
    cout << "A call F()" << endl;
  }
  void callF() {
    cout << "A call callF()" << endl;
    F();
  }
  virtual ~A() {
    cout << "A call ~A()" << endl;
  }
};

class B : public A{
public:
  virtual void F() override {
    cout << "B call F()" << endl;
  }
  void callF() {
    cout << "B call callF()" << endl;
    F();
  }
  virtual ~B() override  {
    cout << "B call ~B()" << endl;
  }
};

int main() {
  cout << "start " << endl;
    A* a = new B();
    a->callF();
    delete a;
    return 0;
}

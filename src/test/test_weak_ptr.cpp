//
// Created by 王聪 on 2020/3/22.
//

#include <iostream>

using namespace std;

class D: public std::enable_shared_from_this<D>
{
public:
  D()
  {
    cout<<"D::D()"<<endl;
  }

  void func()
  {
    cout<<"D::func()"<<endl;
    std::shared_ptr<D> p = shared_from_this();
  }
};

int main()
{
  shared_ptr<D> d(new D);
  d->func();
  return 0;
}
//
// Created by wc on 5/4/19.
//
#include <iostream>
#include "type.h"
#include "../base/timer_heap.h"

using namespace qg;
using namespace std;



int main() {
  TimerHeap th = TimerHeap();
  TimeStamp  t = TimeStamp(0);
  for (int i=30; i>=0; i--) {
	auto fun = [=]() {cout << "runTimer" << i << endl;};
	shared_ptr<Timer> ti(new Timer(addTime(t, i*10000), fun, 0));
	ti->run();
  	th.addTimer(ti);
  }
  cout << th.top() << endl;
  for (int i=0; i<31; i++) {
	shared_ptr<Timer> ti = th.top();
	std::cout << (ti->expire().getUnixTimeStamp()) << endl;
	//ti->run();
    th.popTimer();
  }
  return 0;
}
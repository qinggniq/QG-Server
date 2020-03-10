//
// Created by wc on 5/6/19.
//

#include "event_loop.h"
#include "../base/timer_queue.h"
#include <iostream>
#include <sys/timerfd.h>
using namespace std;
using namespace qg;

int main() {
  std::shared_ptr<Dispatcher> s(new EventLoop());
  TimerQueue tq(s);
  TimeStamp t = TimeStamp::Now();
  for (int i=0; i<=60; i++) {
	auto fun = [=]() {cout << "runTimer" << i << endl;};
	shared_ptr<Timer> ti(new Timer(addTime(t, 2+i), fun, 0));
	//ti->run();
	tq.addTimer(ti);
  }
  s->loop();
}
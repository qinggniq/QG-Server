//
// Created by wc on 5/6/19.
//

#include "../core/event_loop.h"
#include "../base/timer_queue.h"
#include "../util/time_stamp.h"
#include <iostream>
#include <glog/logging.h>
#include "../core/event_handler.h"

using namespace std;
using namespace qg;

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = 1;
  EventLoop* el = new EventLoop();
  EventHandler* h = new EventHandler(el, 1);
  el->registerHandler(h);
  TimeStamp t = TimeStamp::Now();
//  el->runEvery(1, [](){cout << "time" << endl;});
  el->runAt(TimeStamp::Now(), [](){cout << " now "<< endl;});
  el->runAfter(3, [](){cout << " after " << endl;});
  el->loop();
}
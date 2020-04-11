//
// Created by 王聪 on 2020/4/9.
//
#include <unistd.h>
#include "../util/nettool.h"
#include <glog/logging.h>
#include <thread>
int pipes[2];

void threadFunc1() {
  int cnt = 10;
  while (cnt--){
    int one = 1;
    ::write(pipes[1], &one, 1);
  }
}
void threadFunc2() {
  int cnt = 10;
  while (cnt--){
    int one = 1;
    ::write(pipes[1], &one, 1);
  }
}

int main() {
  ::pipe(pipes);
  qg::makeFdNoneBlock(pipes[0]);
  qg::makeFdNoneBlock(pipes[1]);
  std::thread thread1(threadFunc1);
  std::thread thread2(threadFunc2);
  thread1.join();
  thread2.join();
}
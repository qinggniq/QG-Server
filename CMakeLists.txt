cmake_minimum_required(VERSION 3.13)
project(QGServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/glog/0.4.0")

include_directories(core)
include_directories(core/http)
include_directories(util)
include_directories(base)


find_package(glog 0.4.0 REQUIRED)
add_executable(main
        src/core/main.cpp
        src/core/event_loop.cpp
        src/core/event_loop.h
        src/util/log.h
        src/util/type.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/poll/Pollpoller.h
        src/core/poll/Poller.h
        src/core/poll/Pollpoller.cpp
        src/util/rwtool.h
        src/util/nettool.h
        src/base/timer.cpp
        src/base/timer.h
        src/util/time_stamp.cpp
        src/util/time_stamp.h
        src/util/copyable.h
        src/util/noncopyable.h
        src/util/atomic.h
        src/core/socket.cpp
        src/core/socket.h
        src/base/timer_heap.cpp
        src/base/timer_heap.h
        src/base/timer_queue.cpp
        src/base/timer_queue.h
        src/core/tcp_connection.cpp
        src/core/tcp_connection.h
        src/core/server.cpp
        src/core/server.h
        src/util/config.h
        src/base/thread_pool.cpp
        src/base/thread_pool.h
        src/core/accepter.cpp
        src/core/accepter.h)

add_executable(Test

        src/core/event_loop.cpp
        src/core/event_loop.h
        src/util/log.h
        src/util/type.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/poll/Pollpoller.h
        src/core/poll/Poller.h
        src/core/poll/Pollpoller.cpp
        src/util/rwtool.h
        src/base/timer.cpp
        src/base/timer.h
        src/util/time_stamp.cpp
        src/util/time_stamp.h
        src/util/copyable.h
        src/util/noncopyable.h
        src/util/atomic.h
        src/core/socket.cpp
        src/core/socket.h
        src/base/timer_heap.cpp
        src/base/timer_heap.h
        src/test/test_timer_heap.cc)

add_executable(test_timer_heap
        src/test/test_timer_heap.cc
        src/base/timer.cpp
        src/base/timer.h
        src/base/timer_heap.cpp
        src/base/timer_heap.h
        src/util/time_stamp.cpp
        src/util/time_stamp.h
        src/util/type.h
        src/base/timer_queue.cpp
        src/base/timer_queue.h)
add_executable(test_timer_in_reactor
        src/base/timer.cpp
        src/base/timer.h
        src/base/timer_heap.cpp
        src/base/timer_heap.h
        src/util/time_stamp.cpp
        src/util/time_stamp.h
        src/util/type.h
        src/base/timer_queue.cpp
        src/base/timer_queue.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/poll/Pollpoller.h
        src/core/poll/Poller.h
        src/core/poll/Pollpoller.cpp
        src/core/event_loop.h
        src/core/event_loop.cpp
        src/test/test_timer_in_loop.cpp)
add_executable(
        test_acceptor
        src/core/socket.cpp
        src/core/socket.h
        src/core/server.cpp
        src/core/server.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/event_loop.cpp
        src/core/event_loop.h
        src/core/poll/Pollpoller.h
        src/core/poll/Poller.h
        src/util/type.h
        src/util/noncopyable.h
        src/util/config.h)
add_executable(
        test_server
        src/core/event_loop.cpp
        src/core/event_loop.h
        src/util/log.h
        src/util/type.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/poll/Pollpoller.h
        src/core/poll/Poller.h
        src/core/poll/Pollpoller.cpp
        src/util/rwtool.h
        src/util/nettool.h
        src/base/timer.cpp
        src/base/timer.h
        src/util/time_stamp.cpp
        src/util/time_stamp.h
        src/util/copyable.h
        src/util/noncopyable.h
        src/util/atomic.h
        src/core/socket.cpp
        src/core/socket.h
        src/base/timer_heap.cpp
        src/base/timer_heap.h
        src/base/timer_queue.cpp
        src/base/timer_queue.h
        src/core/tcp_connection.cpp
        src/core/tcp_connection.h
        src/core/server.cpp
        src/core/server.h
        src/util/config.h
        src/base/thread_pool.cpp
        src/base/thread_pool.h
        src/core/accepter.cpp
        src/core/accepter.h
        src/test/test_server.cpp
)
add_executable(
        test_weak_ptr
        src/test/test_weak_ptr.cpp
)
target_link_libraries(main glog::glog)
target_link_libraries(test_timer_heap glog::glog)
target_link_libraries(test_timer_in_reactor glog::glog)
target_link_libraries(test_acceptor glog::glog)
target_link_libraries(test_server glog::glog)
target_link_libraries(test_weak_ptr)

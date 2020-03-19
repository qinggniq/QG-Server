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
        src/core/http/http_data.cc
        src/core/http/http_data.h
        src/core/http/http_parser.cc
        src/core/http/http_parser.h
        src/core/main.cpp
        src/core/event_loop.cpp
        src/core/event_loop.h
        src/util/log.h
        src/util/type.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/http/accept_handler_inherit.cpp
        src/core/http/accept_handler_inherit.h
        src/core/http/request_handler_inherit.cpp
        src/core/http/request_handler_inherit.h
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
        src/core/http/http_handler.cpp
        src/core/http/http_handler.h
        src/base/thread_pool.cpp
        src/base/thread_pool.h
        src/core/accepter.cpp
        src/core/accepter.h)

add_executable(Test
        src/test/test_http_paser.cc
        src/core/http/http_data.cc
        src/core/http/http_data.h
        src/core/http/http_parser.cc
        src/core/http/http_parser.h
        src/core/event_loop.cpp
        src/core/event_loop.h
        src/util/log.h
        src/util/type.h
        src/core/event_handler.cpp
        src/core/event_handler.h
        src/core/http/accept_handler_inherit.cpp
        src/core/http/accept_handler_inherit.h
        src/core/http/request_handler_inherit.cpp
        src/core/http/request_handler_inherit.h
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
        src/core/poll/sync_event_demultiplexer.cpp
        src/core/poll/sync_event_demultiplexer.h
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
        src/core/poll/sync_event_demultiplexer.cpp
        src/core/poll/sync_event_demultiplexer.h
        src/util/type.h
        src/util/noncopyable.h
        src/util/config.h
        src/core/http/http_handler.cpp
        src/core/http/http_handler.h)
target_link_libraries(main glog::glog)
target_link_libraries(test_timer_heap glog::glog)
target_link_libraries(test_timer_in_reactor glog::glog)
target_link_libraries(test_acceptor glog::glog)


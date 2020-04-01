cmake_minimum_required(VERSION 3.13)
project(QGServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/glog/0.4.0")

include_directories(core)
include_directories(core/http)
include_directories(util)
include_directories(base)
include_directories(lib)


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
        src/core/accepter.h
        src/core/poll/KqueuePoller.cpp
        src/core/poll/KqueuePoller.h
        src/core/http/http_request.cpp
        src/core/http/http_request.h
        src/lib/http-parser/http_parser.h
        src/lib/http-parser/http_parser.c src/core/http/http_context.cpp src/core/http/http_context.h src/core/http/http_response.cpp src/core/http/http_response.h src/core/http/http_server.cpp src/core/http/http_server.h)

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
        src/test/test_timer_heap.cc src/core/poll/KqueuePoller.cpp src/core/poll/KqueuePoller.h src/test/A.cpp)

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
        src/base/timer_queue.h src/core/poll/KqueuePoller.cpp src/core/poll/KqueuePoller.h)
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
        src/test/test_timer_in_loop.cpp src/core/poll/KqueuePoller.cpp src/core/poll/KqueuePoller.h)
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
        src/util/config.h src/core/poll/KqueuePoller.cpp src/core/poll/KqueuePoller.h)
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
        src/core/poll/KqueuePoller.cpp
        src/core/poll/KqueuePoller.h
        src/core/http/http_request.cpp
        src/core/http/http_request.h
        src/lib/http-parser/http_parser.h
        src/lib/http-parser/http_parser.c
        src/core/http/http_context.cpp
        src/core/http/http_context.h
        src/lib/http-parser/http_parser.c
        src/lib/http-parser/http_parser.h)
add_executable(
        test_weak_ptr
        src/test/test_weak_ptr.cpp
        src/core/poll/KqueuePoller.cpp src/core/poll/KqueuePoller.h)

add_executable(
        test_kqueue
        src/test/test_kqueue.cc
        src/util/time_stamp.cpp
        src/util/time_stamp.h
)
add_executable(test_virtual
         src/test/test_virtual_deconstruct.cpp)

add_executable(test_parser
        src/core/http/test/test_parser.cpp
        src/core/http/http_context.cpp
        src/core/http/http_context.h
        src/core/http/http_request.cpp
        src/core/http/http_request.h
        src/lib/http-parser/http_parser.h
        src/lib/http-parser/http_parser.c)

target_link_libraries(main glog::glog)
target_link_libraries(test_timer_heap glog::glog)
target_link_libraries(test_timer_in_reactor glog::glog)
target_link_libraries(test_acceptor glog::glog)
target_link_libraries(test_server glog::glog)
target_link_libraries(test_kqueue glog::glog)
target_link_libraries(test_weak_ptr)
target_link_libraries(test_virtual)
target_link_libraries(test_parser glog::glog)
set_target_properties(test_server PROPERTIES
        CMAKE_CXX_STANDARD 17)

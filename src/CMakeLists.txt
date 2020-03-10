cmake_minimum_required(VERSION 3.13)
project(QGServer)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/glog/0.4.0")
find_package(glog 0.4.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(main
        core/http/http_data.cc
        core/http/http_data.h
        core/http/http_parser.cc
        core/http/http_parser.h
        core/main.cpp
        core/event_loop.cpp
        core/event_loop.h
        util/log.h
        util/type.h core/event_handler.cpp core/event_handler.h core/accept_handler_inherit.cpp core/accept_handler_inherit.h core/http/request_handler_inherit.cpp core/http/request_handler_inherit.h util/rwtool.h util/nettool.h base/timer.cpp base/timer.h util/time_stamp.cpp util/time_stamp.h util/copyable.h util/noncopyable.h util/atomic.h core/socket.cpp core/socket.h base/timer_heap.cpp base/timer_heap.h base/timer_queue.cpp base/timer_queue.h core/tcp_connection.cpp core/tcp_connection.h core/Server.cpp core/Server.h util/config.h core/http/http_handler.cpp core/http/http_handler.h base/thread_pool.cpp base/thread_pool.h)
add_executable(Test
        test/test_http_paser.cc
        core/http/http_data.cc
        core/http/http_data.h
        core/http/http_parser.cc
        core/http/http_parser.h
        core/event_loop.cpp
        core/event_loop.h
        util/log.h
        util/type.h
        core/event_handler.cpp
        core/event_handler.h
        core/accept_handler_inherit.cpp
        core/accept_handler_inherit.h
        core/http/request_handler_inherit.cpp
        core/http/request_handler_inherit.h
        util/rwtool.h base/timer.cpp
        base/timer.h util/time_stamp.cpp
        util/time_stamp.h util/copyable.h
        util/noncopyable.h util/atomic.h
        core/socket.cpp core/socket.h
        base/timer_heap.cpp
        base/timer_heap.h
        test/test_timer_heap.cc)

add_executable(test_timer_heap
        test/test_timer_heap.cc
        base/timer.cpp
        base/timer.h
        base/timer_heap.cpp
        base/timer_heap.h
        util/time_stamp.cpp
        util/time_stamp.h
        util/type.h
        base/timer_queue.cpp base/timer_queue.h)
add_executable(test_timer_in_reactor
        base/timer.cpp
        base/timer.h
        base/timer_heap.cpp
        base/timer_heap.h
        util/time_stamp.cpp
        util/time_stamp.h
        util/type.h
        base/timer_queue.cpp
        base/timer_queue.h
        core/event_handler.cpp
        core/event_handler.h
        core/poll/sync_event_demultiplexer.cpp
        core/poll/sync_event_demultiplexer.h
        core/event_loop.h
        core/event_loop.cpp
        test/test_timer_in_loop.cpp)
add_executable(
        test_acceptor
        core/socket.cpp
        core/socket.h
        core/Server.cpp
        core/Server.h
        core/event_handler.cpp
        core/event_handler.h
        core/event_loop.cpp
        core/event_loop.h
        core/poll/sync_event_demultiplexer.cpp
        core/poll/sync_event_demultiplexer.h
        util/type.h
        util/noncopyable.h
        util/config.h core/http/http_handler.cpp core/http/http_handler.h)
target_link_libraries(main ${Boost_LIBRARIES} glog::glog)
target_link_libraries(test_timer_heap ${Boost_LIBRARIES} glog::glog)
target_link_libraries(test_timer_in_reactor ${Boost_LIBRARIES} glog::glog)
target_link_libraries(test_acceptor ${Boost_LIBRARIES} glog::glog)

include_directories(core)
include_directories(util)
include_directories(base)



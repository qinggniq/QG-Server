cmake_minimum_required(VERSION 3.10)
project(QG_Server)

set(CMAKE_CXX_STANDARD 11)


include_directories(src/core)
include_directories(src/util)

add_executable(QG_Server
        src/core/http_parser.cc
        src/core/http_parser.h
        src/core/http_data.cc
        src/core/http_data.h
        src/util/log.h
        src/util/type.h
        src/test/test_http_paser.cc src/core/event_loop.cpp src/core/event_loop.h src/core/reactor.cpp src/core/reactor.h src/core/sync_event_demultiplexer.cpp src/core/sync_event_demultiplexer.h)
add_executable(
        unit_test
        src/core/reactor.cpp src/core/reactor.h src/core/sync_event_demultiplexer.cpp src/core/sync_event_demultiplexer.h)

add_executable(
        test_timer
        src/test/test_timer.cc
        src/core/timer.cpp
        src/core/timer.h
        src/core/timer_heap.cpp
        src/core/timer_heap.h
        src/util/time_stamp.cpp
        src/util/time_stamp.h
        src/util/type.h
)
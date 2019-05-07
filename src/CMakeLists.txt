cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS )
add_link_options(-lboost_system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(main
            core/event_loop.cpp
            core/event_loop.h
            core/http_data.cc
            core/http_data.h
            core/http_parser.cc
            core/http_parser.h
            core/main.cpp
            core/reactor.cpp
            core/reactor.h
            core/sync_event_demultiplexer.cpp
            core/sync_event_demultiplexer.h
            util/log.h
            util/type.h core/event_handler.cpp core/event_handler.h core/accept_handler_inherit.cpp core/accept_handler_inherit.h core/event_handler_inherit.cpp core/event_handler_inherit.h core/request_handler_inherit.cpp core/request_handler_inherit.h util/rwtool.h util/nettool.h core/timer.cpp core/timer.h util/time_stamp.cpp util/time_stamp.h util/copyable.h util/noncopyable.h util/atomic.h core/socket.cpp core/socket.h core/timer_heap.cpp core/timer_heap.h core/timer_queue.cpp core/timer_queue.h core/tcp_connection.cpp core/tcp_connection.h core/Server.cpp core/Server.h util/config.h core/http_handler.cpp core/http_handler.h)
    add_executable(Test
            test/test_http_paser.cc
            core/event_loop.cpp
            core/event_loop.h
            core/http_data.cc
            core/http_data.h
            core/http_parser.cc
            core/http_parser.h
            core/reactor.cpp
            core/reactor.h
            core/sync_event_demultiplexer.cpp
            core/sync_event_demultiplexer.h
            util/log.h
            util/type.h
            core/event_handler.cpp
            core/event_handler.h
            core/accept_handler_inherit.cpp
            core/accept_handler_inherit.h
            core/event_handler_inherit.cpp
            core/event_handler_inherit.h
            core/request_handler_inherit.cpp
            core/request_handler_inherit.h
            util/rwtool.h core/timer.cpp
            core/timer.h util/time_stamp.cpp
            util/time_stamp.h util/copyable.h
            util/noncopyable.h util/atomic.h
            core/socket.cpp core/socket.h
            core/timer_heap.cpp
            core/timer_heap.h
            test/test_timer_heap.cc)

        add_executable(test_timer_heap
                test/test_timer_heap.cc
                core/timer.cpp
                core/timer.h
                core/timer_heap.cpp
                core/timer_heap.h
                util/time_stamp.cpp
                util/time_stamp.h
                util/type.h
                core/timer_queue.cpp core/timer_queue.h)
        add_executable(test_timer_in_reactor
                core/timer.cpp
                core/timer.h
                core/timer_heap.cpp
                core/timer_heap.h
                util/time_stamp.cpp
                util/time_stamp.h
                util/type.h
                core/timer_queue.cpp
                core/timer_queue.h
                core/event_handler.cpp
                core/event_handler.h
                core/sync_event_demultiplexer.cpp
                core/sync_event_demultiplexer.h
                core/reactor.h
                core/reactor.cpp
                test/test_timer_in_loop.cpp)
        add_executable(
                test_acceptor
                core/socket.cpp
                core/socket.h
                core/Server.cpp
                core/Server.h
                core/event_handler.cpp
                core/event_handler.h
                core/reactor.cpp
                core/reactor.h
                core/sync_event_demultiplexer.cpp
                core/sync_event_demultiplexer.h
                util/type.h
                util/noncopyable.h
                util/config.h core/http_handler.cpp core/http_handler.h)
    target_link_libraries(main ${Boost_LIBRARIES})
    target_link_libraries(test_timer_heap ${Boost_LIBRARIES})
    target_link_libraries(test_timer_in_reactor ${Boost_LIBRARIES})
    target_link_libraries(test_acceptor ${Boost_LIBRARIES})
endif()

include_directories(core)
include_directories(util)


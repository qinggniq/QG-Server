cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS )

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
            util/type.h core/event_handler.cpp core/event_handler.h)
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
            core/event_handler.cpp core/event_handler.h)
    add_executable(TestStruct
            test/test_http_paser.cc
            )
    target_link_libraries(main ${Boost_LIBRARIES})
endif()

include_directories(core)
include_directories(util)


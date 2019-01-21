cmake_minimum_required(VERSION 3.10)
project(QG_Server)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/core)
include_directories(src/util)

add_executable(QG_Server
        src/core/HttpParser.cc
        src/core/HttpParser.h
        src/core/HttpData.cc
        src/core/HttpData.h
        src/util/log.h
        src/util/type.h
        src/core/main.cpp)
add_executable(
        unit_test
        src/test/test_http_paser.cc
)
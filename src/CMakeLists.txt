set(SRCS
        core/HttpData.cc
        core/HttpParser.cc
        util/type.h
        util/log.h)
include_directories(${PROJECT_SOURCE_DIR}/base)


add_executable(QG-Server ${SRCS})
target_link_libraries(QG-Server libserver_base)


add_subdirectory(util)
add_subdirectory(log)
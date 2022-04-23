# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(MyApplication)

add_definitions(-DASSERT_ENABLE)
add_library(entry SHARED hello.cpp)
target_link_libraries(entry libhilog_ndk.z.so)
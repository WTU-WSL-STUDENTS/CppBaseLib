# CMakeList.txt: vstest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("vstest")


set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__WINDOWS)
add_definitions(-DMSVC_64) 

include_directories(../../include)

add_executable(System.List.Test ../System.List.Test.cpp)
add_executable (System.Net.Sockets.SocketAsync.Test ../System.Net.Sockets.SocketAsync.Test.cpp)
add_executable (System.Threading.ThreadPool.Test ../System.Threading.ThreadPool.Test.cpp)


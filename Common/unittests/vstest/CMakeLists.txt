# CMakeList.txt: vstest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("vstest")


set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__WINDOWS)
add_definitions(-DMSVC_64) 
add_definitions(-D_CRT_SECURE_NO_WARNINGS) 
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(../../include)
include_directories(../)

# add_executable(System.Convert.Test ../System.Convert.Test.cpp)
# add_executable(System.List.Test ../System.List.Test.cpp)
# add_executable (System.Net.Sockets.Socket.Test ../System.Net.Sockets.Socket.Test.cpp)
# add_executable (System.Net.Sockets.SocketAsync.Test ../System.Net.Sockets.SocketAsync.Test.cpp)
# add_executable (System.Threading.ThreadPool.Test ../System.Threading.ThreadPool.Test.cpp)
# add_executable (System.Threading.Thread.Test ../System.Threading.Thread.Test.cpp)
# add_executable (System.Threading.Timer.Test ../System.Threading.Timer.Test.cpp)
# add_executable (System.Threading.Tasks.Task.Test ../System.Threading.Tasks.Task.Test.cpp)

add_executable (IComparable.Test IComparable.Test.cpp)
add_executable (IObserver.Test IObserver.Test.cpp)
add_executable (IEnumerator.Test IEnumerator.Test.cpp)
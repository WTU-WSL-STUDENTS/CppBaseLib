# CMakeList.txt: vstest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("vstest")


set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__WINDOWS)
add_definitions(-DMSVC_64) 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(../../Common/include)

add_executable(PerfectBinaryTree PerfectBinaryTree.Test.cpp)
